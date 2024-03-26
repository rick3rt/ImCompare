#include <vector>
#include <memory>
#include <iostream>
#include <string>

#define LOG(x) std::cout << x << std::endl;

struct Data
{
    std::string name;
    std::string info;

    Data(const std::string &name_, const std::string &info_) : name(name_), info(info_)
    {
        LOG("[Data] Construct")
    }
    ~Data() { LOG("[Data] Desctruct") }
};

class TestClass
{
  public:
    TestClass() { LOG("[TestClass] Construct") }
    ~TestClass() { LOG("[TestClass] Desctruct") }

    void ListData()
    {
        for (auto &data : m_Data)
        {
            LOG(data->name << " " << data->info)
        }
    }

    void AddData(const std::string &name, const std::string &info)
    {
        m_Data.push_back(std::make_shared<Data>(name, info));
    }

  private:
    std::vector<std::shared_ptr<Data>> m_Data;
};

int main()
{

    TestClass tc;
    tc.AddData("name1", "info1");
    tc.AddData("name2", "info2");
    tc.AddData("name3", "info3");

    tc.ListData();

    return 0;
}