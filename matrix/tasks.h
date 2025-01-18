#ifndef __TASKS_
#define __TASKS_

#include <vector>
#include <stdint.h>
#include "IMDconcepts.h"

namespace IMD {
	//Возвращает вектор координат, представляющих позиции сетки в спиральном порядке обхода с учётом начальной позиции.
    std::vector<std::vector<size_t>> centralSpiralGrid(size_t rows, size_t cols, size_t rStart, size_t cStart) {
        std::vector<std::vector<size_t>> result{ {rStart, cStart} }; //Стоим на начальной позиции.
        std::vector<std::vector<short>> directions{ //Вектор направлений обхода сетки.
            {0, 1},
            {1, 0},
            {0, -1},
            {-1, 0} };
        size_t index_direction{ 0 };
        size_t power_extension{ 0 }; //Степень "расширения" сетки.
        size_t count_cells{ rows * cols - 1 }; //Количество необойдённых клеток.
        while (count_cells > 0) {
            if (index_direction == 0 || index_direction == 2) ++power_extension;
            for (size_t i{ 0 }; i < power_extension; ++i) {
                rStart += directions[index_direction][0];
                cStart += directions[index_direction][1];
                //Возможно переполнение памяти.
                if (rStart < rows && cStart < cols) {
                    result.push_back({ rStart, cStart });
                    --count_cells;
                    if (count_cells <= 0) return result;
                }
            }
            index_direction = (index_direction + 1) % 4; //0, 1, 2 и 3
        }
        return result;
	}
}

#endif
