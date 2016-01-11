#ifndef __PFS_NET_INET4_ADDR_HPP__
#define __PFS_NET_INET4_ADDR_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace net {

/**
 * @brief Satisfies Concepts:
 *        @li CopyConstructible
 *        @li CopyAssignable
 */
class DLL_API inet4_addr
{
private:
	static const uint32_t invalid_addr_value;

private:
    uint32_t _addr;

public:
    /**
     * @brief Constructs invalid @c inet4_addr instance.
     */
    inet4_addr ()
		: _addr(invalid_addr_value)
	{}

    /**
     * @brief Constructs inet4_addr from four numeric parts.
     *
     * @details Each of the four numeric parts specifies a byte of the address;
     *          the bytes are assigned in left-to-right order to produce the binary address.
     *
     * @param a First numeric part.
     * @param b Second numeric part.
     * @param c Third numeric part.
     * @param d Fourth numeric part.
     */
    inet4_addr (uint8_t a, uint8_t b, uint8_t c, uint8_t d);


    /**
     * @brief Constructs inet4_addr from three numeric parts.
     *
     * @details Parts @a a and @a b specify the first two bytes of the binary address.
     *          Part @a c is interpreted as a 16-bit value that defines the rightmost
     *          two bytes of the binary address. This notation is suitable
     *          for specifying (outmoded) Class B network addresses.
     *
     * @param a First numeric part
     * @param b Second numeric part
     * @param c Third numeric parts.
     */
    inet4_addr (uint8_t a, uint8_t b, uint16_t c);

    /**
     * @brief Constructs inet4_addr from two numeric parts.
     *
     * @details Part @a a specifies the first byte of the binary address.
     *          Part @a b is interpreted as a 24-bit value that defines
     *          the rightmost three bytes of the binary address.
     *          This notation is suitable for specifying (outmoded)
     *          Class C network addresses.
     *
     * @param a First numeric part
     * @param b Second numeric part
     */
    inet4_addr (uint8_t a, uint32_t b);

    /**
     * @brief Constructs inet4_addr from one numeric part.
     *
     * @param a Numeric part.
     */
    inet4_addr (uint32_t a);

    /**
     * @brief Constructs inet4_addr from string representing the IPv4 address.
     *
     * @details @a s must be in one of the format:
     *             (1) a.b.c.d
     *             (2) a.b.C
     *             (3) a.B
     *             (4) A
     *
     * @param s String representing the IPv4 address.
     */
    inet4_addr (const string & s);


    /**
     * @brief Returns @c true if inet4_addr represents a valid IPv4 address.
     */
    operator bool () const
	{
    	return _addr == invalid_addr_value;
	}

    uint32_t native () const
    {
    	return _addr;
    }
};

}} // pfs:io

namespace pfs {

/**
 * @brief Converts IPv4 address to string.
 *
 * @details The format specification is a null-terminated string and may
 * 		contain special character sequences called conversion specifications,
 *      each of which is introduced by a '%' character and terminated by
 *      some other character known as a conversion specifier character.
 *      All other character sequences are ordinary character sequences.
 *
 *      The characters of ordinary character sequences (including the null byte)
 *      are copied verbatim from format to resulting string. However,
 *      the characters of conversion specifications are replaced as follows:
 *
 *      @li %a First byte of IPv4 address.
 *      @li %A The full 32-bit representation of IPv4 address.
 *      @li %b Second byte of IPv4 address.
 *      @li %B 24-bit representation of last part of IPv4 address
 *      @li %c Third byte of IPv4 address.
 *      @li %C 16-bit representation of last part of IPv4 address
 *      @li %d Fourth byte of IPv4 address.
 *      @li %% '%' character.
 *
 * @param addr Source address for conversion.
 * @param format Conversion format string. If @a format is empty the default format
 *      will be used: "%a.%b.%c.%d".
 * @param base Specifies the radix (8, 10 or 16) for output address parts.
 * @return String representation of IPv4 address. If @a addr is not valid
 *      result will be an empty string.
 *
 * @note 192.0.2.235           decimal
 *       0xC0.0x00.0x02.0xEB   hex
 *       0300.0000.0002.0353   octal
 *       0xC00002EB            decimal
 *       3221226219            hex
 *       030000001353          octal
 *
 *
 */
string to_string (const net::inet4_addr & addr, const string & format, int base);

/**
 * @brief Converts IPv4 address to string with base 10.
 * @param addr Source address for conversion.
 * @param format Conversion format string. If @a format is empty the default format
 * 		will be used: "%a.%b.%c.%d".
 * @return String representation of IPv4 address. If @a addr is not valid
 *      result will be an empty string.
 */
inline string to_string (const net::inet4_addr & addr, const string & format)
{
	return to_string(addr, format, 10);
}

/**
 * @brief Converts IPv4 address to string with format "%a.%b.%c.%d" and base 10.
 *
 * @param addr Source address for conversion.
 * @return String representation of IPv4 address. If @a addr is not valid
 *         result will be an empty string.
 */
inline string to_string (const net::inet4_addr & addr)
{
	return to_string(addr, string(), 10);
}

} //pfs

#endif /* __PFS_NET_INET4_ADDR_HPP__ */
