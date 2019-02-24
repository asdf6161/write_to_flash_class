/*
 * Flashmem.h
 *
 *  Created on: 15 ���. 2019 �.
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
	/* ����������� ��������� ���������� ����� ��������
	 * ���� �� ���������� ��������� FLASH_PAGE_SIZE, �� �������� ������� �������
	 * flash_page_size ����������� � ������
	 * */
	Flashmem(uint16_t num_page);
	virtual ~Flashmem();

	// vars
	uint16_t num_page = 0;			// ����� �������� � ������� ����� ������ ������
	uint16_t page_shift = 0;		// ����� �������� � ������� �������� ������ (���� �������� �� �������� num_page ������ �������� � 4 �������� �� page_shift ����� ����� 3)
	uint32_t main_mem_start = 0;	// ���������� ����� ������ ������
	uint32_t flash_ptr = 0;  		// �������� ������ ������������ ��������� �������� page_num
	const uint32_t flash_page_size = FLASH_PAGE_SIZE_BYTES;  // ������ ����� �������� �� flash

	// data 	- ��������� �� ������������ ������
	// count 	- ���������� ������������ ����, ������ ���� ������ 2
	// ��������������� ���������� ���������� ������
	// ������������� ������� �������� ������ ���� ���������� ������ � � ������
	void flash_mem_write(uint8_t *data, uint32_t count);
	void flash_erase_all_pages();

	// ����������� ����� �������� ����� MAIN_MEM_START_ADDR, ����� �������� � ������ FLASH_PAGE_SIZE_BYTES
	uint32_t get_page_addres(uint16_t num_page);
	// ����������� ����� ����� ��������
	uint32_t get_end_page_addr(uint16_t num_page);

	// �������� � ��������� �������� ������ ������������ �������� page_num
	uint32_t get_flash_ptr();
	void set_flash_ptr(uint32_t addr);

	// ��������� ����������� ������
	uint32_t get_main_mem_start();

private:
	// clear page on flash
	void flash_mem_erase(uint32_t pageAddress);
	void unlock_flash();
	void lock_flash();
	bool check_new_page();
	void enable_flash_programming(bool flag);  // ��������� ��� ��������� ���������������� flash ������
	uint8_t flash_ready();

// �����
public:
	/*���������� str � ������ �������� � ���������� ��������� �� ��*/
	uint8_t *test_write(uint8_t *str, uint16_t len);
};

#endif /* FLASHMEM_H_ */
