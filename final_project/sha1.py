from bitstring import BitArray, BitStream
import pdb

def bit_add32(bs1, bs2):
    b1 = int(bs1.bin, 2)
    b2 = int(bs2.bin, 2)
    b3 = b1 + b2
    bs3 = BitArray(bin(b3))
    while len(bs3) < 32:
        bs3.prepend('0x0')
    return bs3[-32:]


def sha1(message):
    h0 = BitArray('0x67452301')
    h1 = BitArray('0xEFCDAB89')
    h2 = BitArray('0x98BADCFE')
    h3 = BitArray('0x10325476')
    h4 = BitArray('0xC3D2E1F0')

    m1 = len(message) * 8
    hex_str = '0x' + ''.join([str(hex(ord(x)))[2:] for x in message])
    ba = BitArray(hex_str)

    ba += BitArray('0b1')

    while len(ba) % 512 != 448:
        ba += BitArray('0b0')

    size = BitArray(bin(m1))
    while len(size) < 64:
        size.prepend('0b0')

    ba.append(size)
    assert(len(ba) == 512)

    w = [BitArray('0x00000000') for x in range(80)]

    
    for i in range(16):
        w[i] = ba[32 * i:32 * (i + 1)]

#    pdb.set_trace()

    for i in range(16,80):
        w[i] = (w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16])
        w[i].rol(1)
    
    a = BitArray(h0)
    b = BitArray(h1)
    c = BitArray(h2)
    d = BitArray(h3)
    e = BitArray(h4)

    for i in range(80):
        if 0 <= i <= 19:
            f = (b & c) | ((~b) & d)
            k = BitArray('0x5A827999')
        elif 20 <= i <= 39:
            f = b ^ c ^ d
            k = BitArray('0x6ED9EBA1')
        elif 40 <= i <= 59:
            f = (b & c) | (b & d) | (c & d)
            k = BitArray('0x8F1BBCDC')
        elif 60 <= i <= 79:
            f = b ^ c ^ d
            k = BitArray('0xCA62C1D6')

        f = f[-32:] # cut it to 32 bits
        
        rotated_tmp = BitArray(a)
        rotated_tmp.rol(5)
        temp = reduce( bit_add32, [rotated_tmp, f, e, k, w[i]] )
        e = d
        d = c

        rotated_tmp = BitArray(b)
        rotated_tmp.rol(30)
        c = rotated_tmp
        b = a
        a = temp


    h0 = bit_add32(h0, a)
    h1 = bit_add32(h1, b)
    h2 = bit_add32(h2, c)
    h3 = bit_add32(h3, d)
    h4 = bit_add32(h4, e)

    #pdb.set_trace()
    
    hh = str(h0)[2:] + str(h1)[2:] + str(h2)[2:] + str(h3)[2:] + str(h4)[2:]
    return hh


if __name__ == "__main__":

    import argparse
    parser = argparse.ArgumentParser(description='Calculate sha1 of intput')
    parser.add_argument('operand', help='string to calculate sha1 of', type=str, default='', nargs='?')
    args = parser.parse_args()
    
    print sha1(args.operand)
