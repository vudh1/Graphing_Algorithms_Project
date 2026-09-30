#include "project3.h"

#include <filesystem>
#include <iomanip>

namespace fs = std::filesystem;

struct Data {
    int num_nodes;
    int num_edges;
    int diameter;
    double clustering_coefficient;
    std::map<int, int> histogram;
};

Data collect_data(Graph graph) {
    Data data{};
    data.num_nodes = graph.get_num_nodes();
    data.num_edges = graph.get_num_edges();
    data.histogram = get_degree_distribution(graph);
    data.diameter = get_diameter(graph);
    data.clustering_coefficient = get_clustering_coefficient(graph);
    return data;
}

void write_degree_file(const fs::path& path, const std::map<int, int>& histogram) {
    std::ofstream file(path);
    file << "Degree,Frequency\n";
    for (const auto& [degree, frequency] : histogram) {
        file << degree << ',' << frequency << '\n';
    }
}

void append_summary(
    const fs::path& info_path,
    const fs::path& diameter_path,
    const fs::path& clustering_path,
    const Data& data
) {
    std::ofstream(info_path, std::ios::app) << data.num_nodes << ',' << data.num_edges << '\n';
    std::ofstream(diameter_path, std::ios::app) << data.num_nodes << ',' << data.diameter << '\n';
    std::ofstream(clustering_path, std::ios::app)
        << data.num_nodes << ',' << std::setprecision(8) << data.clustering_coefficient << '\n';
}

void initialize_summary_files(const fs::path& output) {
    std::ofstream(output / "er_info.csv") << "Size,Edges\n";
    std::ofstream(output / "er_diameter.csv") << "Size,Diameter\n";
    std::ofstream(output / "er_clustering.csv") << "Size,Clustering Coefficient\n";

    std::ofstream(output / "ba_info.csv") << "Size,Edges\n";
    std::ofstream(output / "ba_diameter.csv") << "Size,Diameter\n";
    std::ofstream(output / "ba_clustering.csv") << "Size,Clustering Coefficient\n";
}

std::vector<int> quick_sizes() {
    return {100, 500, 1000};
}

std::vector<int> full_sizes() {
    return {100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000};
}

int main(int argc, char* argv[]) {
    bool full = false;
    fs::path output = "data_All";

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--full") {
            full = true;
        } else if (arg == "--output" && i + 1 < argc) {
            output = argv[++i];
        } else if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [--full] [--output DIRECTORY]\n";
            return 0;
        } else {
            std::cerr << "Unknown argument: " << arg << '\n';
            return 2;
        }
    }

    fs::create_directories(output);
    initialize_summary_files(output);

    const std::vector<int> sizes = full ? full_sizes() : quick_sizes();

    for (int n : sizes) {
        std::cout << "Erdos-Renyi n=" << n << std::endl;
        Graph er = create_erdos_renyi_graph(n, 2.0 * std::log(static_cast<double>(n)) / n);
        Data er_data = collect_data(er);
        append_summary(
            output / "er_info.csv",
            output / "er_diameter.csv",
            output / "er_clustering.csv",
            er_data
        );
        write_degree_file(output / (std::to_string(n) + "_er_degree.csv"), er_data.histogram);

        std::cout << "Barabasi-Albert n=" << n << std::endl;
        Graph ba = create_barabasi_albert_graph(n, 5);
        Data ba_data = collect_data(ba);
        append_summary(
            output / "ba_info.csv",
            output / "ba_diameter.csv",
            output / "ba_clustering.csv",
            ba_data
        );
        write_degree_file(output / (std::to_string(n) + "_ba_degree.csv"), ba_data.histogram);
    }

    std::cout << "Results written to " << output << std::endl;
    return 0;
}
