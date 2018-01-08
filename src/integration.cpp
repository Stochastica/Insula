#include "integration.hpp"

#include <iostream>
#include <vector>

#include "util/parallel.hpp"

namespace ins
{

void render(Image* const image, TaskParameters const& pp,
            Sampler* const sampler)
{
	assert(image->width > 0 && image->height > 0);
	assert(pp.bucketSize > 0);

	struct Block
	{
		int x0, x1, y0, y1;
	};
	auto bucket = [image, sampler](Block b)
	{
		for (int x = b.x0; x < b.x1; ++x)
			for (int y = b.y0; y < b.y1; ++y)
			{
				assert(x + y * image->width < image->width * image->height);
				Color4 pixel = sampler->at(Vector2i(x, y));
				int i = y * image->width + x;
				image->pixelsR[i] = (uint8_t)(pixel(0) * 0xFF);
				image->pixelsG[i] = (uint8_t)(pixel(1) * 0xFF);
				image->pixelsB[i] = (uint8_t)(pixel(2) * 0xFF);
				image->pixelsA[i] = 0xFF;
			}
	};

	int nBlocksX = image->width / pp.bucketSize + 1;
	int nBlocksY = image->height / pp.bucketSize + 1;
	ThreadPool<Block> tp(bucket, nBlocksX * nBlocksY, pp.nThreads);

	int blockX[nBlocksX + 1];
	int blockY[nBlocksY + 1];
	{
		for (int i = 0; i < nBlocksX; ++i)
			blockX[i] = i * pp.bucketSize;
		blockX[nBlocksX] = image->width;
		for (int i = 0; i < nBlocksY; ++i)
			blockY[i] = i * pp.bucketSize;
		blockY[nBlocksY] = image->height;
	}
	for (int i = 0; i < nBlocksX; ++i)
		for (int j = 0; j < nBlocksY; ++j)
		{
			Block b;
			b.x0 = blockX[i];
			b.x1 = blockX[i + 1];
			b.y0 = blockY[j];
			b.y1 = blockY[j + 1];
			tp.pushTask(b);
		}

	tp.start();
	while (!tp.tasksEmpty())
		std::this_thread::yield();

	tp.halt();
}

} // namespace ins
