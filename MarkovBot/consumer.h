#ifndef __MARKOVBOT_CONSUMER_H
#define __MARKOVBOT_CONSUMER_H

#include <time.h>
#include "Utility.h"

namespace MarkovBot
{
	/*
	 * Consumes the markov data structure to output "Markovian" text.
	 */
	class Consumer
	{
	public:
		Consumer(markov graph);
		void swap(markov graph);
		std::string generate_text(int phrases, int count);
	private:
		markov markov_graph;
		std::string find_starting_place();
	};
}

#endif