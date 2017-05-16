#!/usr/bin/env python3

import sys
import click
import io
import glob
import os
import subprocess
import testgen

algorithms_dir = os.path.dirname(os.path.abspath(__file__)) + "/../build"
algorithms = [
        (r"$\mathtt{STUPID}$" , "stupid"),
        (r"$\REC{MST}$" , "rec_mst"),
        (r"$\REC{MStT}$" , "rec_mstt"),
        (r"$\REC{SPT}$" , "rec_spt"),
        (r"$\MCAMH$" , "mcamh"),
        (r"$\CAMH$" , "camh"),
        (r"$\mathtt{MCAMHLE}$" , "mcamhle"),
        (r"$\mathtt{MStTThm10}$" , "msttthm10"),
        (r"$\mathtt{MStTMax}$" , "msttmax")
        ]

def run_algo(function, cam, algo):
    solution = subprocess.run([os.path.join(algorithms_dir, algo), function], input=cam, stdout=subprocess.PIPE).stdout
    result = subprocess.run([os.path.join(algorithms_dir, "eval"), function], input=cam + solution, stdout=subprocess.PIPE).stdout
    # print(cam + solution)
    # print(os.path.join(algorithms_dir, "eval"), function)
    # print(result)
    return float(result)

def run_algos(function, cam):
    results = []
    for _, algo in algorithms:
        results.append(run_algo(function, cam, algo))
    return results

def generate_table(results, file=sys.stdout):
    print(r"\begin{tabular}{" + "l" * len(algorithms) + r"}", file=file)
    print(" & ".join(x[0] for x in algorithms) + r"\\", file=file)
    for filename, sub_results in results:
        print(r"%", filename)
        print(" & ".join(str(x) for x in sub_results), r"\\", file=file)
    print(r"\end{tabular}", file=file)


@click.command()
@click.argument('function')
@click.argument('test_files', nargs=-1)
def run(function, test_files):
    results = list()
    for test_filename in test_files:
        with open(test_filename, 'rb') as test_file:
            try:
                sub_results = run_algos(function, test_file.read())
                results.append((test_filename, sub_results))
            except ValueError:
                print(test_filename, file=sys.stderr)
    generate_table(results)
    
if __name__ == '__main__':
    run()
