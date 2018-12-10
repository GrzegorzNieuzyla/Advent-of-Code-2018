#!/usr/bin/env python3

import sys
import math


def parse_data():
    """
    Parse data and return list of lists [x, y, velocity_x, velocity_y]
    """
    data = []
    for line in sys.stdin:
        split = line.strip().split(',')
        x = int(split[0].split('<')[1])
        y = int(split[1].split('>')[0])
        vx = int(split[1].split('<')[1])
        vy = int(split[2][:-1])
        data.append([x, y, vx, vy])
    return data


def iterate(data):
    for point in data:
        point[0] += point[2]
        point[1] += point[3]


def bounding_box(data):
    """
    Returns bounding box of points as (left, top, width, height)
    """
    left, right, top, bot = data[0][0], data[0][0], data[0][1], data[0][1]
    for point in data:
        if left > point[0]:
            left = point[0]
        if right < point[0]:
            right = point[0]
        if top > point[1]:
            top = point[1]
        if bot < point[1]:
            bot = point[1]
    return (left, top, right - left + 1, bot - top + 1)


def print_points(data, bb):
    """
    prints points on grid, required monospace font
    """
    points = set()
    for point in data:
        points.add((point[0], point[1]))

    for y in range(bb[3]):
        for x in range(bb[2]):
            if (x + bb[0], y + bb[1]) in points:
                print('#', end='')
            else:
                print(' ', end='')
        print()
    print()


def main():
    data = parse_data()
    second = 0
    last_height = math.inf
    while True:
        second += 1
        iterate(data)
        bb = bounding_box(data)

        # print grid if height of bounding box is small enough
        if bb[3] <= 20:
            print("Second: ", second)
            print_points(data, bb)

        if last_height < bb[3]:
            break
        last_height = bb[3]


if __name__ == "__main__":
    main()
