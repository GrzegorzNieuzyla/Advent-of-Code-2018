#!/usr/bin/env python3
import sys


def parse_date():
    """
    Parse data and return state as list of bools
    and transformations as dict [pattern] -> result 
    """
    lines = sys.stdin.read().split('\n')
    init_state = [c == '#' for c in lines.pop(0).split()[2]]
    transformations = {}
    for line in filter(None, lines):
        split = line.split(' => ')
        transformations[tuple(c == '#' for c in split[0])] = split[1] == '#'
    return init_state, transformations


def simulate(state, transformations, generations):
    """
    Return state after simulation and offset from 0 position from init state 
    """
    offset = 0
    for g in range(generations):
        while state[:2] != [False, False]:
            state = [False] + state
            offset -= 1
        while state[-2:] != [False, False]:
            state.append(False)

        newstate = []
        for i in range(len(state)):
            if i > 1:
                pattern = state[i-2:i]
            else:
                pattern = [False, False]
            pattern.append(state[i])
            if i < len(state) - 2:
                pattern += state[i+1:i+3]
            else:
                pattern += [False, False]

            newstate.append(transformations[tuple(pattern)])
        state = newstate
    return state, offset


def get_count(state, offset):
    res = 0
    for i, b in enumerate(state):
        if b:
            res += (i + offset)
    return res


def main():
    init, trans = parse_date()

    state, offset = simulate(init[:], trans, 20)
    print("50 generations: ", get_count(state, offset))

    target = 50000000000
    pred = 2000  # prediction after how many generations changes will stabilize
    state, offset = simulate(init[:], trans, pred)

    print(target, "generations: ", get_count(state, offset + (target - pred)))


if __name__ == '__main__':
    main()
