import os, sys

def extract(cpio_path, out_dir, substrs):
    n = 0
    with open(cpio_path, 'rb') as f:
        while True:
            hdr = f.read(76)
            if len(hdr) < 76:
                break
            if hdr[:6] != b'070707':
                raise ValueError(f'bad magic at {f.tell() - 76}: {hdr[:6]!r}')
            namesize = int(hdr[59:65], 8)
            filesize = int(hdr[65:76], 8)
            name = f.read(namesize - 1).decode('utf-8', 'replace')
            f.read(1)
            if name == 'TRAILER!!!':
                break
            if any(s in name for s in substrs) and filesize > 0:
                out = os.path.join(out_dir, name.replace('./', '').replace('/', '_'))
                remaining = filesize
                with open(out, 'wb') as o:
                    while remaining > 0:
                        chunk = f.read(min(1 << 20, remaining))
                        o.write(chunk)
                        remaining -= len(chunk)
                print(f'extracted {name} ({filesize})', flush=True)
                n += 1
            else:
                f.seek(filesize, 1)
    return n

if __name__ == '__main__':
    extract(sys.argv[1], sys.argv[2], sys.argv[3].split(','))
