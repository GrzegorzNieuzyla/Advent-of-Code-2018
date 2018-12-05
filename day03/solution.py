#!/usr/bin/env python3
import sys


def get_claims():
    claims = []

    # Parse line of format '#ID @ X,Y: WxH'
    for line in sys.stdin:
        claim = {}
        line = line.strip()
        parts = line.split(' ')

        claim['id'] = int(parts[0][1:])

        coords = parts[2][:-1].split(',')
        claim['x'] = int(coords[0])
        claim['y'] = int(coords[1])

        size = parts[3].split('x')
        claim['w'] = int(size[0])
        claim['h'] = int(size[1])
        claims.append(claim)
    return claims


def get_surface(claims):
    """
    returns dict: (x,y) -> number of claims for that square inch
    """

    res = {}
    for claim in claims:
        for x in range(claim['w']):
            for y in range(claim['h']):
                posx, posy = claim['x'] + x, claim['y'] + y
                if (posx, posy) not in res:
                    res[(posx, posy)] = 1
                else:
                    res[(posx, posy)] += 1
    return res


def overlaps(claim, surface):
    """
    Checks if claim overlaps with any other on the surface
    """
    for x in range(claim['w']):
        for y in range(claim['h']):
            posx, posy = claim['x'] + x, claim['y'] + y
            if surface[(posx, posy)] > 1:
                return True

    return False


def main():
    claims = get_claims()
    surface = get_surface(claims)
    count = sum(i > 1 for i in surface.values())
    print('Count: ', count)

    for claim in claims:
        if not overlaps(claim, surface):
            print("Not overlapping: #" + str(claim['id']))


if __name__ == "__main__":
    main()
