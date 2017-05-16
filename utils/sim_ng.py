#!/usr/bin/env python3

import os
import io
import testgen
import testrun
import click

def generate_num_chunks(k, graph, num_chunks_range, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element):
    suffix = "numc.maxe{:d}.minc{:d}.maxc{:d}".format(max_edge_weight, min_chunk_size, max_chunk_size)
    inputs = list()
    for num_chunks in num_chunks_range:
        inputs.append((num_chunks, testgen.generate_from_topobench_bytes(
            graph, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, k=k)))
    return (suffix, inputs)

def generate_max_edge(k, graph, num_chunks, max_edge_weight_range, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element):
    suffix = "maxe.numc{:d}.minc{:d}.maxc{:d}".format(num_chunks, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element)
    inputs = list()
    for max_edge_weight in max_edge_weight_range:
        inputs.append((max_edge_weight, testgen.generate_from_topobench_bytes(
            graph, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, k=k)))
    return (suffix, inputs)

def generate_avg_chunk_size(k, graph, num_chunks, max_edge_weight, avg_chunk_size_range, avg_chunk_size_delta, min_chunk_element, max_chunk_element):
    suffix = "avgc.numc{:d}.maxe{:d}".format(num_chunks, max_edge_weight)
    inputs = list()
    for avg_chunk_size in avg_chunk_size_range:
        inputs.append((avg_chunk_size, testgen.generate_from_topobench_bytes(
            graph, num_chunks, max_edge_weight, avg_chunk_size - avg_chunk_size_delta, avg_chunk_size + avg_chunk_size_delta, min_chunk_element, max_chunk_element, k=k)))
    return (suffix, inputs)

def generate_files(graph_full_name, inputs_data, normalized=False, function_name='dict', output_dir='.'):
    suffix, inputs = inputs_data
    algorithm_results = dict()
    for _, algo in testrun.algorithms:
            algorithm_results[algo] = list()
            for value, samples in inputs:
                results = list()
                for sample in samples:
                    results.append(testrun.run_algo(function_name, sample, algo))
                algorithm_results[algo].append((value, results))

    for algo, results in algorithm_results.items():
        if normalized and algo == "stupid":
            continue
        norm_part = 'norm.' if normalized else ''
        with open('{}/may2017.{}.{}.{}.{}{}.csv'.format(output_dir, function_name, graph_full_name, algo, norm_part, suffix), 'w') as output:
            for i, (value, results) in enumerate(results):
                if normalized:
                    result = sum(x / y for x, y in zip(results, algorithm_results['stupid'][i][1])) / len(results)
                else:
                    result = sum(results) / len(results)
                print(value, result, file=output)

@click.command()
@click.option('--func', default="dict", help="function name")
@click.option('--dir', default=".", help="output dir")
@click.option('--num', default=1000, help='number of experiments')
@click.argument('graph_file', type=click.File())
def gensmth(graph_file, func, dir, num):
    dirname, file_name = os.path.split(graph_file.name)
    graph_name = os.path.basename(dirname) + "." + file_name[:-4][6:]
    graph = io.StringIO(graph_file.read())
    generate_files(graph_name, generate_max_edge(num, graph, 10, range(1, 50, 1), 10, 30, 1, 50), normalized=False, function_name=func, output_dir=dir)
    generate_files(graph_name, generate_avg_chunk_size(num, graph, 20, 100, range(10, 100, 2), 10, 1, 150), normalized=False, function_name=func, output_dir=dir)
    generate_files(graph_name, generate_num_chunks(num, graph, range(20, 50, 1), 15, 10, 30, 1, 50),normalized=False, function_name=func, output_dir=dir)
    generate_files(graph_name, generate_max_edge(num, graph, 10, range(1, 50, 1), 10, 30, 1, 50), normalized=True, function_name=func, output_dir=dir)
    generate_files(graph_name, generate_avg_chunk_size(num, graph, 20, 100, range(10, 100, 2), 10, 1, 150), normalized=True, function_name=func, output_dir=dir)
    generate_files(graph_name, generate_num_chunks(num, graph, range(20, 50, 1), 15, 10, 30, 1, 50),normalized=True, function_name=func, output_dir=dir)


    #generate_files(graph_name, generate_max_edge(100, graph, 10, range(1, 50, 1), 10, 30))
    #generate_files(graph_name, generate_avg_chunk_size(100, graph, 20, 100, range(10, 100, 2), 10))
    #generate_files(graph_name, generate_num_chunks(100, graph, range(20, 50, 1), 15, 10, 30))

if __name__ == '__main__':
    gensmth()

# for func in dict choosebest setunion
# do
#     for f in tests/swdc_ring.graph.32.4/cam.nc10.maxe10.minc1.maxc10.mine1.maxe50.txt tests/jelly_fish.graph.20.20.30.20.16.10/cam.nc10.maxe10.minc1.maxc10.mine1.maxe50.txt tests/hypercube.graph.80.8/cam.nc10.maxe10.minc1.maxc10.mine1.maxe50.txt
#     do
#         python3 sim_ng.py --func $func --dir csv_$func $f &
#     done
# done
# wait

