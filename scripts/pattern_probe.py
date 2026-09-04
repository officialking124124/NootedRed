import sys

def find_masked(data, pat, mask):
    hits = []
    n, m = len(pat), len(data)
    plen = min(len(pat), len(mask))
    first = pat[0]
    fmask = mask[0] if mask else 0xFF
    for i in range(m - n):
        if (data[i] & fmask) != (first & fmask):
            continue
        ok = True
        for j in range(1, plen):
            mj = mask[j]
            if mj and (data[i + j] & mj) != (pat[j] & mj):
                ok = False
                break
        if ok:
            hits.append(i)
    return hits

def parse(pattern_str):
    b = []
    for tok in pattern_str.split(','):
        tok = tok.strip().rstrip('U')
        if tok.startswith('0x') or tok.startswith('0X'):
            b.append(int(tok, 16))
        elif tok:
            b.append(int(tok))
    return b

if __name__ == '__main__':
    data = open(sys.argv[1], 'rb').read()
    pat = parse(sys.argv[2])
    mask = parse(sys.argv[3]) if len(sys.argv) > 3 else [0xFF] * len(pat)
    hits = find_masked(data, pat, mask)
    print(f'{len(hits)} hit(s): ' + ', '.join(f'{h:#x}' for h in hits[:12]))
