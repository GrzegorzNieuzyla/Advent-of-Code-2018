#!/usr/bin/env python3
import sys
from pprint import pprint
from functools import cmp_to_key
from copy import deepcopy
import pygame


def reading_order(p1, p2):
    if p1[1] < p2[1]:
        return -1
    if p1[1] > p2[1]:
        return 1
    return p1[0] - p2[0]


def get_neighbours(k):
    """
    Return cells adjacent to given position in reading order
    """
    return ((k[0], k[1] - 1), (k[0] - 1, k[1]),
            (k[0] + 1, k[1]), (k[0], k[1] + 1))


def parse_data():
    """
    Parse input data as graph in adjacency list
    returns graph, initial board state and character positions
    """
    board = {}
    characters = {}
    for y, line in enumerate(sys.stdin):
        for x, char in enumerate(line.strip()):
            if char != '#':
                board[(x, y)] = char
                if char in ('E', 'G'):
                    characters[(x, y)] = {'type': char, 'hp': 200, 'ap': 3}

    graph = {}
    for k in board:
        graph[k] = []
        neigbours = get_neighbours(k)
        for x, y in neigbours:
            if (x, y) in board:
                graph[k].append((x, y))
    return graph, board, characters


def get_shortest_paths(pos, target, graph, board):
    """
    Returns shortest paths from (x, y) pos to (x, y) target utilizing BFS algorithm
    """
    open_set = [pos]
    closed_set = set()

    prev = {pos: None}

    while open_set:
        node = open_set.pop(0)

        if node == target:
            path = []
            while prev[node]:
                path.append(node)
                node = prev[node]
            path.append(node)
            path.reverse()
            return path

        for child in graph[node]:
            if board[child] != '.':
                continue
            if child in closed_set:
                continue

            if child not in open_set:
                prev[child] = node
                open_set.append(child)
        closed_set.add(node)
    return None


def move(pos, characters, graph, board):
    """
    Have unit perform a move if necessery
    """
    enemy_t = 'G' if characters[pos]['type'] == 'E' else 'E'
    path = None

    # check if there is need to move
    for adj in get_neighbours(pos):
        if adj in characters and characters[adj]['type'] == enemy_t:
            return pos

    in_battle = False

    # determine the optimal path
    for k in filter(lambda c: characters[c]['type'] == enemy_t, characters):
        in_battle = True
        adjacent = get_neighbours(k)

        for adj in filter(lambda p: p in board and board[p] == '.', adjacent):
            current = get_shortest_paths(pos, adj, graph, board)
            if current:
                if ((not path or len(current) < min_len)
                        or (len(current) == min_len and
                            reading_order(adj, path[-1]) < 0)):

                    min_len = len(current)
                    path = current

    if not in_battle:
        return None

    if not path:
        return False

    # move character on board
    characters[path[1]] = characters[pos]
    board[path[1]] = characters[pos]['type']
    del characters[pos]
    board[pos] = '.'

    return path[1]


class ElvesLost(Exception):
    pass


def attack(pos, characters, graph, board, break_on_lose=False):
    """
    Have character on (pos) perform an attack
    If an elf dies and break_on_lose is set exception is raised
    """

    enemy_t = 'G' if characters[pos]['type'] == 'E' else 'E'
    adj = get_neighbours(pos)
    target = None
    # determine the most optimal target
    for a in filter(lambda a: a in characters, reversed(adj)):
        if characters[a]['type'] == enemy_t:
            if not target or target['hp'] >= characters[a]['hp']:
                target = characters[a]
                target_pos = a

    if target:
        target['hp'] -= characters[pos]['ap']
        if target['hp'] <= 0:
            if target['type'] == 'E' and break_on_lose:
                # an elf has died
                raise ElvesLost()
            board[target_pos] = '.'
            del characters[target_pos]
    return True


def round(graph, board, characters, break_on_lose=False):
    """
    Performs a single round i.e. move and attack by every character
    If an elf dies and break_on_lose is set exception is raised
    """
    for pos in sorted(characters, key=cmp_to_key(reading_order)):
        if pos not in characters:
            continue
        new_pos = move(pos, characters, graph, board)
        if new_pos is None:
            return False
        if new_pos:
            attack(new_pos, characters, graph, board, break_on_lose)
    return True


TILE_SIZE = 30
INPUT_WIDTH = 32
INPUT_HEIGHT = 32


def draw_board(board, screen):
    for pos in board:
        if board[pos] == '.':
            color = (0x38, 0x38, 0x38)
        if board[pos] == 'E':
            color = (0xff, 0, 0,)
        if board[pos] == 'G':
            color = (0, 0xff, 0)
        pygame.draw.rect(screen, color, pygame.Rect(
            pos[0] * TILE_SIZE, pos[1] * TILE_SIZE, TILE_SIZE, TILE_SIZE))


def main():
    g, b, c = parse_data()
    screen = pygame.display.set_mode(
        (TILE_SIZE * INPUT_WIDTH, TILE_SIZE * INPUT_HEIGHT))
    clock = pygame.time.Clock()

    # part 1
    graph = deepcopy(g)
    board = deepcopy(b)
    characters = deepcopy(c)

    round_count = 0
    while round(graph, board, characters):
        screen.fill((0, 0, 0))
        clock.tick(20)
        draw_board(board, screen)
        pygame.display.flip()
        round_count += 1

    total_hp = 0
    for char in characters.values():
        total_hp += char['hp']
    print("Part 1: ", total_hp * round_count)

    # part 2
    ap = 4
    step = 8
    while True:
        graph = deepcopy(g)
        board = deepcopy(b)
        characters = deepcopy(c)

        # set attack power for every elf
        for char in characters.values():
            if char['type'] == 'E':
                char['ap'] = ap

        round_count = 0
        try:
            while round(graph, board, characters, True):
                screen.fill((0, 0, 0))
                clock.tick(20)
                draw_board(board, screen)
                pygame.display.flip()
                round_count += 1
        except ElvesLost:
            print("Elves lost with AP:", ap)
            ap += step
            continue

        # decrease step
        if step > 1:
            ap -= (step - 1)
            step = 1
            continue

        # Elves has won, calculate final result
        total_hp = 0
        for char in characters.values():
            total_hp += char['hp']
        print("Part 2: ", total_hp * round_count, "with AP:", ap)
        break


if __name__ == '__main__':
    main()
