#!/usr/bin/env python3

import io
import click
import os
import sys
import random
import numpy as np

@click.group()
def initial():
    pass

def generate_random_graph(num_vertices, num_chunks, max_weight, file=sys.stdout):
    num_edges = random.randrange(num_vertices - 1, num_vertices * (num_vertices - 1) // 2)

    print(num_vertices, num_edges, file=file)
    connected = [0]
    disconnected = list(range(1, num_vertices))
    for i in range(num_vertices - 1):
        source = random.choice(connected)
        destination = random.choice(disconnected) 
        print(source, destination, random.randrange(1, max_weight + 1), file=file)
        connected.append(destination)
        disconnected.remove(destination)
    for i in range(num_vertices - 1, num_edges):
        print(random.randrange(num_vertices), random.randrange(num_vertices), 
                random.randrange(1, max_weight), file=file)

def generate_graph_from_topobench(graph_file, max_edge_weight, file=sys.stdout):
    num_vertices, num_edges = (int(x) for x in graph_file.readline().split())
    print(num_vertices, num_edges, file=file)
    for i in range(num_edges):
        source, destination, _ = [int(x) for x in graph_file.readline().split()]
        print(source, destination, random.randrange(1, max_edge_weight + 1), file=file)
    return (num_vertices, num_edges)

def generate_chunks(num_vertices, num_chunks, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, file=sys.stdout):
    print(num_chunks, file=file)
    for i in range(num_chunks):
        chunk_size = random.randrange(min_chunk_size, max_chunk_size + 1)
        chunk_elements = sorted(list(min_chunk_element + np.random.choice(max_chunk_element - min_chunk_element +1, chunk_size, replace=False)))
        print(random.randrange(1, num_vertices), chunk_size, " ".join([ "%d" % x for x in chunk_elements]), file=file)
    print(0, file=file) 

def generate_from_topobench_bytes(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, file=sys.stdout, k = None):
    if k is not None:
        samples = list()
        print("Generating %d, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element" % k, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element)
        for _ in range(k):
            graph_file.seek(0)
            samples.append(generate_from_topobench_bytes(
                graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element))
            # print('\t.')
        return samples
    with io.StringIO() as testIO:
        generate_from_topobench(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, file=testIO)
        return testIO.getvalue().encode('ASCII')

def generate_from_topobench(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, file=sys.stdout):
    num_vertices,_ = generate_graph_from_topobench(graph_file, max_edge_weight, file=file)
    generate_chunks(num_vertices, num_chunks, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, file=file)

def generate_from_topobench_meta(graph_filename, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, output_dir):
    base_name = "cam.nc{:d}.maxe{:d}.minc{:d}.maxc{:d}.mine{:d}.maxe{:d}.txt".format(num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element)
    file_name = os.path.join(output_dir, base_name)
    with open(graph_filename) as graph_file, open(file_name, 'w+') as output:
        generate_from_topobench(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, file=output)

@initial.command()
@click.option('--num-chunks', default=10, help='number of chunks')
@click.option('--max-edge-weight', default=10, help='max weight')
@click.option('--min-chunk-size', default=1, help='min chunk size')
@click.option('--max-chunk-size', default=10, help='max chunk size')
@click.option('--min-chunk-element', default=1, help='min chunk element')
@click.option('--max-chunk-element', default=50, help='max chunk element')
@click.option('--output-dir', help='output directory')
@click.argument('graph_file', type=click.Path(exists=True))
def gen_from_topo(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, output_dir):
    if output_dir is not None:
        generate_from_topobench_meta(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, output_dir)
    else:
        with open(graph_file) as graph_file:
            generate_from_topobench(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element)

if __name__ == '__main__':
    initial()
