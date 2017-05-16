#!/usr/bin/env python3
import testgen
import glob
import os
import click

cur_directory = os.path.dirname(os.path.abspath(__file__))

topology_base_dir = "/home/snikolenko/networking/topologies/topobench"

topology_dirs = {
        "dragonfly" : os.path.join(topology_base_dir, "dragonfly"),
        "jelly_fish" : os.path.join(topology_base_dir, "jellyfish_heterserver"),
        "fat_tree" : os.path.join(topology_base_dir, "fattree"),
        "swdc_ring" : os.path.join(topology_base_dir, "swdc_ring"),
        "VL2" : os.path.join(topology_base_dir, "VL2"),
        "hypercube" : os.path.join(topology_base_dir, "hypercube")
}

def generate_all_tests(output_dir, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element):
    for name, graph_dir in topology_dirs.items():
        for graph_filename in glob.glob(graph_dir + "/graph*.txt"):
            directory_name = os.path.join(output_dir, name + "." + os.path.basename(graph_filename)[:-4])
            if not os.path.exists(directory_name):
                os.makedirs(directory_name)
            testgen.generate_from_topobench_meta(graph_filename, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element, directory_name)

@click.group()
def initial():
    pass

@initial.command()
@click.option('--output_dir', default=os.path.join(cur_directory, "tests"))
@click.option('--num-chunks', default=10, help='number of chunks')
@click.option('--max-edge-weight', default=10, help='max weight')
@click.option('--min-chunk-size', default=1, help='min chunk size')
@click.option('--max-chunk-size', default=10, help='max chunk size')
@click.option('--min-chunk-element', default=1, help='min chunk element')
@click.option('--max-chunk-element', default=50, help='max chunk element')
def generate(output_dir, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element):
    generate_all_tests(output_dir, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, min_chunk_element, max_chunk_element)

@initial.command()
def generate_some():
    for num_chunks in range(10, 50, 10):
        for max_edge_weight in range(10, 40, 10):
            for chunk_mid in range(20, 100, 10):
                generate_all_tests('tests', num_chunks, max_edge_weight, chunk_mid - 10, chunk_mid + 20, 1, 50)

if __name__ == '__main__':
    initial()

