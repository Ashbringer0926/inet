// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef __INET_MACADDRESSTABLE_H
#define __INET_MACADDRESSTABLE_H

#include "inet/linklayer/common/MacAddress.h"
#include "inet/linklayer/ethernet/switch/IMacAddressTable.h"

namespace inet {

/**
 * This module handles the mapping between ports and MAC addresses. See the NED definition for details.
 * NOTE that ports (portno parameters) are actually the corresponding ID of the port interface.
 * i.e. this is an interfaceId and NOT an index of the some kind in a gate vector.
 */
class INET_API MacAddressTable : public cSimpleModule, public IMacAddressTable
{
  protected:
    struct AddressEntry
    {
        unsigned int vid = 0;    // VLAN ID
        int portno = -1;    // Input port (interface id)
        simtime_t insertionTime;    // Arrival time of Lookup Address Table entry
        AddressEntry() {}
        AddressEntry(unsigned int vid, int portno, simtime_t insertionTime) :
            vid(vid), portno(portno), insertionTime(insertionTime) {}
    };

    friend std::ostream& operator<<(std::ostream& os, const AddressEntry& entry);

    struct MacCompare
    {
        bool operator()(const MacAddress& u1, const MacAddress& u2) const { return u1.compareTo(u2) < 0; }
    };

    typedef std::map<MacAddress, AddressEntry, MacCompare> AddressTable;
    typedef std::map<unsigned int, AddressTable *> VlanAddressTable;

    simtime_t agingTime;    // Max idle time for address table entries
    simtime_t lastPurge;    // Time of the last call of removeAgedEntriesFromAllVlans()
    AddressTable *addressTable = nullptr;    // VLAN-unaware address lookup (vid = 0)
    VlanAddressTable vlanAddressTable;    // VLAN-aware address lookup

  protected:

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
    virtual void updateDisplayString() const;

    /**
     * @brief Returns a MAC Address Table for a specified VLAN ID
     */
    AddressTable *getTableForVid(unsigned int vid);

  public:

    MacAddressTable();
    ~MacAddressTable();

  public:
    // Table management

    /**
     * @brief For a known arriving port, V-TAG and destination MAC. It finds out the port where relay component should deliver the message
     * @param address MAC destination
     * @param vid VLAN ID
     * @return Output port for address, or -1 if unknown.
     */
    virtual int getPortForAddress(const MacAddress& address, unsigned int vid = 0) override;

    /**
     * @brief Register a new MAC address at AddressTable. portno is the interface ID of the port
     * @return True if refreshed. False if it is new.
     */
    virtual bool updateTableWithAddress(int portno, const MacAddress& address, unsigned int vid = 0) override;

    /**
     *  @brief Clears portno cache (portno is the interface ID of the port)
     */
    // TODO: find a better name
    virtual void flush(int portno) override;

    /**
     *  @brief Prints cached data
     */
    virtual void printState() override;

    /**
     * @brief Copy cache from portA to portB port
     */
    virtual void copyTable(int portA, int portB) override;

    /**
     * @brief Remove aged entries from a specified VLAN
     */
    virtual void removeAgedEntriesFromVlan(unsigned int vid = 0) override;
    /**
     * @brief Remove aged entries from all VLANs
     */
    virtual void removeAgedEntriesFromAllVlans() override;

    /*
     * It calls removeAgedEntriesFromAllVlans() if and only if at least
     * 1 second has passed since the method was last called.
     */
    virtual void removeAgedEntriesIfNeeded() override;

    /**
     * Pre-reads in entries for Address Table during initialization.
     */
    virtual void readAddressTable(const char *fileName) override;

    /**
     * For lifecycle: initialize entries for the vlanAddressTable by reading them from a file (if specified by a parameter)
     */
    virtual void initializeTable() override;

    /**
     * For lifecycle: clears all entries from the vlanAddressTable.
     */
    virtual void clearTable() override;

    /*
     * Some (eg.: STP, RSTP) protocols may need to change agingTime
     */
    virtual void setAgingTime(simtime_t agingTime) override;
    virtual void resetDefaultAging() override;
};

} // namespace inet

#endif // ifndef __INET_MACADDRESSTABLE_H

