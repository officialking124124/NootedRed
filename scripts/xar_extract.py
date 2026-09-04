import struct, sys, zlib
import xml.etree.ElementTree as ET

def tag(e):
    return e.tag.split('}')[-1]

def extract_file(xar_path, want_name, out_path):
    with open(xar_path, 'rb') as f:
        hdr = f.read(28)
        size, version, toc_c, toc_u, cksum = struct.unpack('>HHQQI', hdr[4:28])
        toc = zlib.decompress(f.read(toc_c))
        heap_off = 28 + toc_c
    root = ET.fromstring(toc)
    for e in root.iter():
        if tag(e) != 'file':
            continue
        name = None
        off = ln = sz = None
        style = None
        for c in e:
            t = tag(c)
            if t == 'name':
                name = c.text
            elif t == 'data':
                for d in c:
                    td = tag(d)
                    if td == 'offset':
                        off = int(d.text)
                    elif td == 'length':
                        ln = int(d.text)
                    elif td == 'size':
                        sz = int(d.text)
                    elif td == 'encoding':
                        style = d.get('style')
        if name == want_name:
            with open(xar_path, 'rb') as f:
                f.seek(heap_off + off)
                data = f.read(ln)
            with open(out_path, 'wb') as o:
                o.write(data)
            print(f'{name}: heap offset {heap_off + off}, stored {ln}, uncompressed {sz}, style {style}')
            return True
    return False

if __name__ == '__main__':
    if not extract_file(sys.argv[1], sys.argv[2], sys.argv[3]):
        print('NOT FOUND')
