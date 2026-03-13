/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_arena_allocator.cc                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilyanar <ilyanar@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 14:30:43 by ilyanar           #+#    #+#             */
/*   Updated: 2026/03/13 17:48:57 by ilyanar          ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hh"

int testArenaAllocator(){
	lpp::unique_chrono chrono("arena_allocator");
	lpp::arena_allocator alloc;

	lpp::memento::Snapshot save;
	lpp::worker_pool *worker = alloc.create<lpp::worker_pool>(10);
	std::shared_ptr io(std::make_unique<IoStat>(1, "mgm", 1, 1));

	{
		// for(size_t i = 0; i < 100; i++)
		// 	worker->addJob(io);
		// while(worker->size() > 50)
		// 	;
		// save = io->save();
		worker->wait();
	}

	IoStat tmpIo(1, "eos", 1 , 1);
	tmpIo.load(save);
	lpp::cout << *io << std::endl;
	lpp::cout << tmpIo << std::endl;

	try{
	auto arena(std::move(alloc));
	}catch(std::system_error &e){
		lpp::cout << e.what() << std::endl;
	}

	return 0;
}
