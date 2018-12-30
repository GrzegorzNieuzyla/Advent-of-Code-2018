#!/usr/bin/env python3
import sys
import re
from functools import cmp_to_key
from copy import deepcopy


class Unit:
    def __init__(self, count, hp, res, ad, type_, init):
        self.count = count
        self.hp = hp
        self.res = res
        self.ad = ad
        self.initiative = init
        self.type = type_
        self.attacked = False
        self.target = None

    def ep(self):
        """ Get effective power """
        return self.count * self.ad

    def reset(self):
        """ Reset targets """
        self.attacked = False
        self.target = None


def parse_data():
    """
    Parse data and return list of immunity units and list of infection units
    """
    lines = sys.stdin.read().split('\n')
    imm_group = []
    inf_group = []
    current = None
    pattern = re.compile(
        r'([0-9]*) units each with ([0-9]*) hit points (\(.*\) )?with an attack' +
        r' that does ([0-9]*) (.*) damage at initiative ([0-9]*)')

    for line in filter(lambda l: l.strip(), lines):
        if line.startswith('Infection'):
            current = inf_group
        elif line.startswith('Immune'):
            current = imm_group
        else:
            match = re.search(pattern, line)
            current.append(Unit(
                int(match.group(1)),
                int(match.group(2)),
                (match.group(3).strip()[1:-1] if match.group(3)
                 else {'immune': [], 'weak': []}),
                int(match.group(4)),
                match.group(5),
                int(match.group(6))
            ))

    for unit in imm_group + inf_group:
        if isinstance(unit.res, str):
            res = {'immune': [], 'weak': []}
            for r in unit.res.split(';'):
                type_, attacks = r.split(' to ')
                res[type_.strip()] += attacks.split(', ')
            unit.res = res
    return imm_group, inf_group


def compare_target(u1, u2):
    """
    Comparison for order of target selection
    """
    ep1 = u1.ep()
    ep2 = u2.ep()
    if ep1 != ep2:
        return ep2 - ep1
    return u2.initiative - u1.initiative


def select_targets(units, enemies):
    """
    Select targets based on the rules
    """
    for unit in sorted(units, key=cmp_to_key(compare_target)):
        max_dmg = -1
        for enemy in filter(lambda u: not u.attacked, enemies):
            if unit.type in enemy.res['immune']:
                continue

            dmg = unit.ep()
            if unit.type in enemy.res['weak']:
                dmg *= 2
            if dmg >= max_dmg:
                if dmg == max_dmg:
                    if enemy.ep() == unit.target.ep():
                        if enemy.initiative <= unit.target.initiative:
                            continue
                    elif enemy.ep() < unit.target.ep():
                        continue
                max_dmg = dmg
                unit.target = enemy
        if unit.target:
            unit.target.attacked = True


def attack(imm, inf):
    """
    Let all units attack based on previous target selection
    """
    for unit in sorted(imm + inf, key=lambda u: u.initiative, reverse=True):
        if unit.count > 0 and unit.target:
            dmg = unit.ep()
            if unit.type in unit.target.res['weak']:
                dmg *= 2
            unit.target.count -= dmg // unit.target.hp

    for unit in imm + inf:
        unit.reset()


def simulate(immunity, infection, boost=0):
    """
    Simulate until one side is wiped out or to a draw
    """
    for unit in immunity:
        unit.ad += boost

    while immunity and infection:
        last_count = sum([u.count for u in immunity + infection])

        select_targets(immunity, infection)
        select_targets(infection, immunity)
        attack(immunity, infection)
        immunity = list(filter(lambda u: u.count > 0, immunity))
        infection = list(filter(lambda u: u.count > 0, infection))
        # handle draw
        if sum([u.count for u in immunity + infection]) == last_count:
            break

    imm_count = sum([u.count for u in immunity])
    inf_count = sum([u.count for u in infection])
    return imm_count, inf_count


def main():
    immunity, infection = parse_data()

    # Part 1
    imm, inf = simulate(deepcopy(immunity), deepcopy(infection))
    print('Part 1:\n', 'Immune system:', imm, ' Infection: ', inf)

    # Part 2
    boost = 1
    while True:
        imm, inf = simulate(deepcopy(immunity), deepcopy(infection), boost)
        if inf == 0:
            break
        boost += 1
    print('Part 2:', imm)


if __name__ == '__main__':
    main()
