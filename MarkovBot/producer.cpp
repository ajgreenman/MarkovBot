#include "producer.h"

using MarkovBot::Producer;

Producer::Producer(void)
	: words()
{

}

Producer::~Producer(void)
{

}

/*
 * Parses the input into a markov graph and writes it to a file.
 */
bool Producer::generate_markov(std::string output_name, std::vector<std::string> file_names)
{
	std::map<std::string, std::vector<std::string>> graph;
	std::map<std::string, std::vector<std::string>> temp_graph;

	for(auto &file : file_names)
	{
		temp_graph.clear();

		if(MarkovBot::Utility::is_markov(file))
		{
			if(!MarkovBot::Utility::parse_markov_file(file, temp_graph))
			{
				return false;
			}
		} 
		else
		{
			if(!parse_file(file, words))
			{
				return false;
			}

			create_markov_graph(words, temp_graph);
		}

		MarkovBot::Utility::combine_graphs(graph, temp_graph);
	}

	return write_file(get_output_name(output_name), graph);
}

/*
 * Parses the input file into an array of individual words.
 */
bool Producer::parse_file(std::string file_name, std::vector<std::string> &w)
{
	std::ifstream ifs;
	ifs.open(file_name);
	if(!ifs.good())
	{
		std::cout << "Unable to read " << file_name << "." << std::endl;
		return false;
	}
	
	std::cout << "Reading " << file_name << "..." << std::endl;

	// Read entire contents of the file into a single string.
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	w = MarkovBot::Utility::split_string_to_vector(content);

	std::cout << "Successfully read " << file_name << "." << std::endl;

	return true;
}

/*
 * Transforms an array of words into a markov graph.
 */
void Producer::create_markov_graph(std::vector<std::string> w, std::map<std::string, std::vector<std::string>> &graph)
{
	std::cout << "Generating markov graph..." << std::endl;

	for(int i = 0; i < w.size() - 1; ++i)
	{
		if(graph.find(w[i]) == graph.end())
		{
			graph[w[i]] = std::vector<std::string>();
		}
		
		graph[w[i]].push_back(w[i + 1]);
	}
	
	std::cout << "Markov graph succesfully created." << std::endl;
}

/*
 * Writes a markov graph to a file.
 */
bool Producer::write_file(std::string output_name, const std::map<std::string, std::vector<std::string> > &graph)
{
	std::ofstream ofs(output_name);

	if(!ofs.is_open())
	{
		std::cout << "Unable to write to " << output_name << "." << std::endl;
		return false;
	}

	std::cout << "Writing final markov graph to " << output_name << "..." << std::endl;

	for(auto &words : graph)
	{
		ofs << words.first << "|";
		for(auto &word : words.second)
		{
			ofs << word << " ";
		}
		ofs << std::endl;
	}

	std::cout << "Finish writing to " << output_name << "." << std::endl;
	ofs.close();
	return true;
}

/*
 * Strips out file extensions and appends the markov file extension.
 */ 
std::string Producer::get_output_name(std::string output_name)
{
	std::size_t pos = output_name.find(".");

	std::string ret_val = output_name.substr(0, pos);
	ret_val.append(".markov");

	return ret_val;
}