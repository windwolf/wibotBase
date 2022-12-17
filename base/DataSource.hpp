//
// Created by zhouj on 2022/12/10.
//

#ifndef WWMOTOR_LIBS_WWBASE_BASE_DATASOURCE_HPP_
#define WWMOTOR_LIBS_WWBASE_BASE_DATASOURCE_HPP_
#include "base.hpp"
namespace wibot
{

	class DataSource
	{
	 public:
		virtual uint32_t get_data() = 0;
	};

	class MemoryDataSource : public DataSource
	{
	 public:
		MemoryDataSource(uint32_t* data) : _data(data)
		{
		}
		uint32_t get_data() override;
	 private:
		uint32_t* _data;
	};

	class DummyDataSource : public DataSource
	{
	 public:
		uint32_t get_data() override;

		static DummyDataSource instance;
	};

} // wibot

#endif //WWMOTOR_LIBS_WWBASE_BASE_DATASOURCE_HPP_
