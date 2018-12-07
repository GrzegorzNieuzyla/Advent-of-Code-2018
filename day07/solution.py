#!/usr/bin/env python3

import sys
from copy import deepcopy


def get_requirements():
    """
    Parse input into mapping: Point -> set of prerequisites
    """
    requirements = {}
    for line in sys.stdin:
        words = line.strip().split(' ')
        X, Y = words[1], words[7]
        if X not in requirements:
            requirements[X] = set()
        if Y not in requirements:
            requirements[Y] = set()

        requirements[Y].add(X)
    return requirements


def next_task(req, keys):
    for k in keys:
        if not req[k]:
            del req[k]
            keys.remove(k)
            return k
    return None


def execution_order(req):
    result = ''
    keys = sorted(req.keys())
    while req:
        t = next_task(req, keys)
        result += t
        # remove completed task from each requirement list
        for p in req:
            if t in req[p]:
                req[p].remove(t)
    return result


def simulate(req):
    workers = {1: [0, 0], 2: [0, 0], 3: [0, 0], 4: [0, 0], 5: [0, 0]}
    second = 0
    keys = sorted(req.keys())
    while req:
        # assign jobs
        for w in filter(lambda w: workers[w][0] == 0, workers):
            t = next_task(req, keys)
            if not t:
                break
            workers[w] = [ord(t) - 4, t]  # 60 + value of letter, current job
        # remove completed jobs if any
        for w in filter(lambda w: workers[w][0], workers):
            workers[w][0] -= 1
            if workers[w][0] == 0:
                for p in req:
                    if workers[w][1] in req[p]:
                        req[p].remove(workers[w][1])
        second += 1

    # add remaining seconds
    for w in workers:
        second += workers[w][0]
    return second


def main():
    req = get_requirements()
    order = execution_order(deepcopy(req))

    print("Part 1:", order)

    print("Part 2:", simulate(req))


if __name__ == "__main__":
    main()
