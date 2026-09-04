import lzma, struct, sys

def decode(path, out_path):
    n = total = 0
    with open(path, 'rb') as f, open(out_path, 'wb') as o:
        assert f.read(4) == b'pbzx', 'not pbzx'
        f.read(8)  # declared chunk size (0x1000000)
        while True:
            hdr = f.read(16)
            if len(hdr) < 16:
                break
            decomp, comp = struct.unpack('>QQ', hdr)
            if comp == 0:
                break
            data = f.read(comp)
            if data[:6] == b'\xfd7zXZ\x00':
                o.write(lzma.decompress(data, format=lzma.FORMAT_XZ))
            elif comp == decomp:  # stored uncompressed chunk
                o.write(data)
            else:
                raise ValueError(f'chunk {n}: neither xz nor stored (comp={comp:#x} decomp={decomp:#x})')
            total += decomp
            n += 1
            if n % 128 == 0:
                print(f'  chunk {n}, {total/1e6:.0f} MB out', flush=True)
    return n, total

if __name__ == '__main__':
    n, sz = decode(sys.argv[1], sys.argv[2])
    print(f'decoded {n} chunks -> {sz} bytes')
