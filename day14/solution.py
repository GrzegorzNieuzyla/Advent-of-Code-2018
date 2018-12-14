#!/usr/bin/env python3

INPUT = 556061


def part_1(count, init):
    res = [*init]
    worker = [0, 1]  # workers position

    while len(res) < count:
        s = res[worker[0]] + res[worker[1]]
        if s < 10:
            res.append(s)
        else:
            res += [s // 10, s % 10]

        worker[0] += (1 + res[worker[0]])
        worker[1] += (1 + res[worker[1]])

        worker[0] %= len(res)
        worker[1] %= len(res)
    return res


def part_2(seq, init):
    seq = [int(c) for c in str(seq)]
    res = [*init]
    worker = [0, 1]

    while True:
        s = res[worker[0]] + res[worker[1]]
        if s < 10:
            res.append(s)
        else:
            res += [s // 10, s % 10]

        worker[0] += (1 + res[worker[0]])
        worker[1] += (1 + res[worker[1]])

        worker[0] %= len(res)
        worker[1] %= len(res)

        if len(res) >= len(seq):
            end = res[-(len(seq)+1):]
            if end[:-1] == seq:
                return len(res) - len(seq) - 1
            if end[1:] == seq:
                return len(res) - len(seq)


def main():
    res = part_1(INPUT + 10, [3, 7])
    print('Part 1:', ''.join(map(str, res[INPUT:INPUT+10])))

    res = part_2(INPUT, [3, 7])
    print('Part 2:', res)


if __name__ == "__main__":
    main()
