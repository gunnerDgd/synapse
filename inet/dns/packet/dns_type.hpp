#pragma once
#include <iostream>
#include <vector>
#include <string>

namespace synapse {
namespace network {
namespace dns     {

    using answer_parser   = std::string(*)(uint16_t, char*, char*);
    // uint16_t : Answer Type
    // void*    : Answer Data
    // char*    : Answer Packet Raw Pointer

    enum dns_type : uint16_t
    {
        invalid_type = 0,
        a = 1,
        ns = 2,
        md = 3,
        mf = 4,
        cname = 5,
        soa = 6,
        mb = 7,
        mg = 8,
        mr = 9,
        null = 10,
        wks = 11,
        ptr = 12,
        hinfo = 13,
        minfo = 14,
        mx = 15,
        txt = 16,
        rp = 17,
        afsdb = 18,
        x25 = 19,
        isdn = 20,
        rt = 21,
        nsap = 22,
        nsap_ptr = 23,
        sig = 24,
        key = 25,
        px = 26,
        gpos = 27,
        aaaa = 28,
        loc = 29,
        nxt = 30,
        eid = 31,
        nimloc = 32,
        srv = 33,
        atma = 34,
        naptr = 35,
        kx = 36,
        cert = 37,
        a6 = 38,
        dname = 39,
        sink = 40,
        opt = 41,
        apl = 42,
        ds = 43,
        sshfp = 44,
        ipseckey = 45,
        rrsig = 46,
        nsec = 47,
        dnskey = 48,
        dhcid = 49,
        nsec3 = 50,
        nsec3param = 51,
        tlsa = 52,
        smimea = 53,
        hip = 55,
        ninfo = 56,
        rkey = 57,
        talink = 58,
        cds = 59,
        cdnskey = 60,
        openpgpkey = 61,
        csync = 62,
        spf = 99,
        uinfo = 100,
        uid = 101,
        gid = 102,
        unspec = 103,
        nid = 104,
        l32 = 105,
        l64 = 106,
        lp = 107,
        eui48 = 108,
        eui64 = 109,
        tkey = 249,
        tsig = 250,
        ixfr = 251,
        axfr = 252,
        mailb = 253,
        maila = 254,
        any_type = 255,
        uri = 256,
        caa = 257,
        avc = 258,
        ta = 32768,
        dlv = 32769
    };

    enum dns_class : uint16_t
    {
	    invalid_class = 0,	/*%< Cookie. */
	    in = 1,		/*%< Internet. */
	    unsupported = 2,		/*%< unallocated/unsupported. */
	    chaos = 3,		/*%< MIT Chaos-net. */
	    hs = 4,		/*%< MIT Hesiod. */
	/* Query class values which do not appear in resource records */
	    none = 254,	/*%< for prereq. sections in update requests */
	    any_class = 255		/*%< Wildcard match. */
    };

    enum dns_opcode : uint16_t
    {
	    query = 0,		/*%< Standard query. */
	    iquery = 1,	/*%< Inverse query (deprecated/unsupported). */
	    status = 2,	/*%< Name server status query (unsupported). */
				/* Opcode 3 is undefined/reserved. */
	    notify = 4,	/*%< Zone change notification. */
	    update = 5,	/*%< Zone update message. */
    };

    enum dns_rcode
    {
	    noerror = 0,	/*%< No error occurred. */
	    formerr = 1,	/*%< Format error. */
	    servfail = 2,	/*%< Server failure. */
	    nxdomain = 3,	/*%< Name error. */
	    notimpl = 4,	/*%< Unimplemented. */
	    refused = 5,	/*%< Operation refused. */
	/* these are for BIND_UPDATE */
	    yxdomain = 6,	/*%< Name exists */
	    yxrrset = 7,	/*%< RRset exists */
	    nxrrset = 8,	/*%< RRset does not exist */
	    notauth = 9,	/*%< Not authoritative for zone */
	    notzone = 10,	/*%< Zone of record different from zone section */
	/* The following are EDNS extended rcodes */
	    badvers = 16,
	/* The following are TSIG errors */
	    badsig = 16,
	    badkey = 17,
	    badtime = 18
    };

}
}
}