import struct, sys

LC_SYMTAB = 0x2

def symbols(path):
    with open(path, 'rb') as f:
        data = f.read()
    magic, cputype, cpusub, filetype, ncmds, sizeofcmds, flags = struct.unpack("<IiiIIII", data[:28])
    assert magic == 0xFEEDFACF, f'not a 64-bit mach-o: {magic:#x}'
    off = 32
    symoff = nsyms = stroff = strsize = None
    for _ in range(ncmds):
        cmd, cmdsize = struct.unpack('<II', data[off:off + 8])
        if cmd == LC_SYMTAB:
            symoff, nsyms, stroff, strsize = struct.unpack('<IIII', data[off + 8:off + 24])
            break
        off += cmdsize
    if symoff is None:
        return []
    strtab = data[stroff:stroff + strsize]
    out = []
    for i in range(nsyms):
        e = data[symoff + i * 16:symoff + (i + 1) * 16]
        n_strx, n_type, n_sect, n_desc, n_value = struct.unpack('<IBBHQ', e)
        end = strtab.find(b'\x00', n_strx)
        name = strtab[n_strx:end].decode('utf-8', 'replace')
        if name:
            out.append((name, n_value, n_type))
    return out

if __name__ == '__main__':
    path, filt = sys.argv[1], sys.argv[2]
    syms = symbols(path)
    print(f'{path}: {len(syms)} symbols')
    hits = [s for s in syms if filt in s[0]]
    for name, val, typ in hits[:int(sys.argv[3]) if len(sys.argv) > 3 else 80]:
        print(f'{val:#012x} {typ:#04x} {name}')
    print(f'-- {len(hits)} matches for {filt!r}')
