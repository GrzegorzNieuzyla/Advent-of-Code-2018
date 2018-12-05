#!/usr/bin/env python3
import sys
from datetime import datetime


def get_records():
    """
    Returns list of (datetime, info text) tuples sorted by date
    """
    res = []
    for line in sys.stdin:
        parts = line.strip().split('] ')
        date = datetime.strptime(parts[0], '[%Y-%m-%d %H:%M')
        res.append((date, parts[1]))

    return sorted(res, key=lambda x: x[0])


def get_guards_data(records):
    """
    Parses data to dict: guard id -> list of tuples of (asleep, wake)
    for each day on duty
    """
    res = {}
    id = -1
    times = []
    fall_time = 0
    for record in records:
        if record[1].startswith('Guard #'):
            if id > 0 and times:
                if id not in res:
                    res[id] = []
                res[id].append(tuple(times))
            id = int(record[1].split(' ')[1][1:])
            times = []
        elif record[1].startswith('falls'):
            fall_time = record[0].minute
        else:
            times.append((fall_time, record[0].minute))
    return res


def most_asleep(data):
    """
    Gets id and data of guard with most time asleep
    """
    max_total = 0
    max_id = -1
    for id, times in data.items():
        total = 0
        for day in times:
            for time in day:
                total += time[1] - time[0]
        if total > max_total:
            max_total = total
            max_id = id
    return max_id, data[max_id]


def get_minute(guard):
    """
    returns minute in which guard was asleep the most, count
    """
    minutes = {}
    for day in guard:
        for time in day:
            for m in range(time[0], time[1]):
                if m not in minutes:
                    minutes[m] = 0
                minutes[m] += 1
    max_min = 0
    max_times = 0
    for minute, times in minutes.items():
        if times > max_times:
            max_min = minute
            max_times = times
    return max_min, max_times


def main():
    records = get_records()
    data = get_guards_data(records)
    id, times = most_asleep(data)
    minute, _ = get_minute(times)
    print("Part 1:", id, '*', minute, '=', minute*id)

    max_count = 0
    for guard_id in data:
        minute, count = get_minute(data[guard_id])
        if count > max_count:
            max_count = count
            max_id = guard_id
            max_minute = minute

    print("Part 2:", max_id, '*', max_minute, '=', max_minute*max_id)


if __name__ == '__main__':
    main()
