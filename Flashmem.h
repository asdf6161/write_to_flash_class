/*
 * Flashmem.h
 *
 *  Created on: 15 €нв. 2019 г.
 *      Author: yura
 *  have code - https://habr.com/ru/post/213771/
 */

/*Example
 * uint8_t *res = flashmem.test_write((uint8_t *)"Hellow flash mem\0", 17);
 * */

#ifndef FLASHMEM_H_
#define FLASHMEM_H_

#include "stdint.h"

#ifdef STM32F103xB
#include "stm32f103xb.h"
#define FLASH_PAGE_SIZE_BYTES 1024  // bytes
#define PAGE_CNT 64
#endif
#ifdef STM32F303xC
#include "stm32f303xc.h"
#define FLASH_PAGE_SIZE_BYTES 2048  // bytes
#define PAGE_CNT 128
#endif

#define MAIN_MEM_START_ADDR 0x08000000

class Flashmem {
public:
	/*  онструктор принимает пор€дковый номер страницы
	 * ≈сли не определена директива FLASH_PAGE_SIZE, то придетс€ указать вручную
	 * flash_page_size ”казываетс€ в байтах
	 * */
	Flashmem(uint16_t num_page);
	virtual ~Flashmem();

	// vars
	uint16_t num_page = 0;			// Ќомер страницы с которой будет начата запись
	uint16_t page_shift = 0;		// Ќомер страницы в которую ведетьс€ запись (если записать на страницу num_page данные размеров в 4 страницы то page_shift бужет равна 3)
	uint32_t main_mem_start = 0;	// јбсолютный адрес начала записи
	uint32_t flash_ptr = 0;  		// —мещение адреса относительно указанной страницы page_num
	const uint32_t flash_page_size = FLASH_PAGE_SIZE_BYTES;  // –азмер одной страницы во flash

	// data 	- указатель на записываемые данные
	// count 	- количество записываемых байт, должно быть кратно 2
	// ѕоследовательно записывает переданные данные
	// јвтоматически очищает страницу пам€ти если происходит запись в еЄ начало
	void flash_mem_write(uint8_t *data, uint32_t count);
	void flash_erase_all_pages();

	// –асчитывает адрес страницы через MAIN_MEM_START_ADDR, номер страницы и размер FLASH_PAGE_SIZE_BYTES
	uint32_t get_page_addres(uint16_t num_page);
	// –асчитывает адрес конца страницы
	uint32_t get_end_page_addr(uint16_t num_page);

	// ”тановка и получение смещени€ адреса относительно страницы page_num
	uint32_t get_flash_ptr();
	void set_flash_ptr(uint32_t addr);

	// ѕолучение абсолютного адреса
	uint32_t get_main_mem_start();

private:
	// clear page on flash
	void flash_mem_erase(uint32_t pageAddress);
	void unlock_flash();
	void lock_flash();
	bool check_new_page();
	void enable_flash_programming(bool flag);  // –азрешает или запрещает программирование flash пам€ти
	uint8_t flash_ready();

// “есты
public:
	/*«аписывает str в начало страницы и возвращает указатель на неЄ*/
	uint8_t *test_write(uint8_t *str, uint16_t len);
};

#endif /* FLASHMEM_H_ */
