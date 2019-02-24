/*
 * Flashmem.cpp
 *
 *  Created on: 15 янв. 2019 г.
 *      Author: yura
 */

#include <Flashmem.h>

Flashmem::Flashmem(uint16_t num_page) {
	this->num_page = num_page;
	this->main_mem_start = this->get_page_addres(this->num_page + this->page_shift);
}

Flashmem::~Flashmem() {
	// pass
}

void Flashmem::flash_mem_write(uint8_t *data, uint32_t count) {
	// Todo - Реализовать запись в несколько страниц
	this->unlock_flash();
//	this->flash_mem_erase(this->main_mem_start);
	uint32_t i;

	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}

	this->enable_flash_programming(true);

	for (i = 0; i < count; i += 2) {

		this->check_new_page();  // Todo Оптимизировать
		*(volatile uint16_t *)(this->main_mem_start + this->flash_ptr) =
				(((uint16_t)data[i + 1]) << 8) + data[i];
		while (!(FLASH->SR & FLASH_SR_EOP));
		FLASH->SR = FLASH_SR_EOP;
		this->flash_ptr += 2;
	}

	this->enable_flash_programming(false);

	this->lock_flash();
}

bool Flashmem::check_new_page(){
	if (this->flash_ptr >= this->flash_page_size * this->page_shift){
		this->enable_flash_programming(false);
		this->flash_mem_erase(this->get_page_addres(this->num_page + this->page_shift));
		this->page_shift++;
		this->enable_flash_programming(true);
		return true;
	}
	return false;
}

void Flashmem::enable_flash_programming(bool flag){
	if(flag){
		FLASH->CR |= FLASH_CR_PG;
	} else {
		FLASH->CR &= ~(FLASH_CR_PG);
	}
}

//Функция стирает ВСЕ страницы. При её вызове прошивка самоуничтожается
void Flashmem::flash_erase_all_pages(void) {
	FLASH->CR |= FLASH_CR_MER; //Устанавливаем бит стирания ВСЕХ страниц
	FLASH->CR |= FLASH_CR_STRT; //Начать стирание
	while(!this->flash_ready()) // Ожидание готовности.. Хотя оно уже наверное ни к чему здесь...
		FLASH->CR &= FLASH_CR_MER;
}

uint32_t Flashmem::get_page_addres(uint16_t num_page){
	return MAIN_MEM_START_ADDR | (FLASH_PAGE_SIZE_BYTES * num_page);
}

uint32_t Flashmem::get_end_page_addr(uint16_t num_page){
	return this->flash_page_size * num_page;
}

uint32_t Flashmem::get_flash_ptr(){
	return this->flash_ptr;
}

void Flashmem::set_flash_ptr(uint32_t addr){
	this->flash_ptr = addr;
}

uint32_t Flashmem::get_main_mem_start(){
	return this->main_mem_start;
}

void Flashmem::unlock_flash() {
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
}

void Flashmem::lock_flash() {
	FLASH->CR |= FLASH_CR_LOCK;
}

uint8_t Flashmem::flash_ready(){
	return !(FLASH->SR & FLASH_SR_BSY);
}

void Flashmem::flash_mem_erase(uint32_t pageAddress) {
	while (!this->flash_ready());

	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR |= FLASH_SR_EOP;
	}

	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = pageAddress;
	FLASH->CR |= FLASH_CR_STRT;

	while (!(FLASH->SR & FLASH_SR_EOP));

	FLASH->SR |= FLASH_SR_EOP;
	FLASH->CR &= ~FLASH_CR_PER;
}
