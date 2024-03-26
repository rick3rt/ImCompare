#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <regex>
#include <set>

struct DirectoryNode
{
    std::string FullPath;
    std::string FileName;
    bool IsDirectory = false;
    std::vector<DirectoryNode> Children;
};

void RecursivelyAddDirectoryNodes(DirectoryNode &parentNode,
                                  std::filesystem::directory_iterator directoryIterator)
{
    for (const std::filesystem::directory_entry &entry : directoryIterator)
    {
        DirectoryNode &childNode = parentNode.Children.emplace_back();
        childNode.FullPath = entry.path().u8string();
        childNode.FileName = entry.path().filename().u8string();
        if (childNode.IsDirectory = entry.is_directory(); childNode.IsDirectory)
            RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
    }

    auto moveDirectoriesToFront = [](const DirectoryNode &a, const DirectoryNode &b) {
        return (a.IsDirectory > b.IsDirectory);
    };
    std::sort(parentNode.Children.begin(), parentNode.Children.end(), moveDirectoriesToFront);
}

DirectoryNode CreateDirectryNodeTreeFromPath(const std::filesystem::path &rootPath)
{
    DirectoryNode rootNode;
    rootNode.FullPath = rootPath.u8string();
    rootNode.FileName = rootPath.filename().u8string();
    if (rootNode.IsDirectory = std::filesystem::is_directory(rootPath); rootNode.IsDirectory)
        RecursivelyAddDirectoryNodes(rootNode, std::filesystem::directory_iterator(rootPath));

    return rootNode;
}

void RecursivelyGetFiles(const DirectoryNode &node, std::vector<DirectoryNode> &files)
{
    if (!node.IsDirectory)
    {
        files.push_back(node);
        return;
    }

    for (const DirectoryNode &child : node.Children)
    {
        RecursivelyGetFiles(child, files);
    }
}

class RegexComparer
{
  public:
    RegexComparer() {}
    ~RegexComparer() {}

    void SetRegexString(const std::string &regex)
    {
        m_RegexString = regex;
        // try to compile regex
        try
        {
            m_Regex = std::regex(m_RegexString);
            m_RegexValid = true;
        }
        catch (const std::regex_error &e)
        {
            std::cout << "Regex error: " << e.what() << std::endl;
            m_RegexValid = false;
        }
    }

    const std::vector<std::set<std::string>> &GetOptions() const { return m_Options; }

    void SetFiles(const std::vector<std::string> &files) { m_Files = files; }
    const bool &GetRegexValid() const { return m_RegexValid; }
    void Compute()
    {
        // the regular expression to test
        size_t num_groups = m_Regex.mark_count();
        m_Options.clear();
        // get the different options matching the groups of the regular expression
        for (size_t i = 0; i < num_groups; i++)
        {
            std::set<std::string> group_options;
            for (auto &file : m_Files)
            {
                std::smatch match;
                std::regex_search(file, match, m_Regex);
                group_options.insert(match[i + 1]);
            }
            m_Options.push_back(group_options);
        }
    }

  private:
    std::string m_RegexString;
    std::regex m_Regex;
    bool m_RegexValid = false;
    std::vector<std::set<std::string>> m_Options;
    std::vector<std::string> m_Files;
};

int main()
{
    std::cout << "Hello World!" << std::endl;

    DirectoryNode rootNode = CreateDirectryNodeTreeFromPath(
        "C:/Users/rickw/LocalDocuments/Code/C++/DataExplorer/resource/example/folders");

    // collect only files from root node
    std::vector<DirectoryNode> files;
    RecursivelyGetFiles(rootNode, files);

    // convert DirectoryNode vector to std::string, with stripping the root path
    std::vector<std::string> filepathStrings;
    for (auto &file : files)
    {
        std::string filepath = file.FullPath;
        filepath.erase(0, rootNode.FullPath.size() + 1);
        filepathStrings.push_back(filepath);
    }

    // display files
    for (auto &file : filepathStrings)
        std::cout << "file: " << file << std::endl;

    RegexComparer reC;
    reC.SetRegexString(R"((Medium\d)\\(Apod\d)\\\w{4}(i\d))");
    reC.SetRegexString(R"((Medium\d)\\(Apod\d)\\\w{4}(i\d))");
    reC.SetFiles(filepathStrings);
    reC.Compute();
    auto options = reC.GetOptions();

    // // the regular expression to test
    // std::string re_string = R"((Medium\d)\\(Apod\d)\\\w{4}(i\d))";
    // std::regex re(re_string);
    // size_t num_groups = re.mark_count();

    // // get the different options matching the groups of the regular expression
    // std::vector<std::set<std::string>> options;
    // for (size_t i = 0; i < num_groups; i++)
    // {
    //     std::set<std::string> group_options;
    //     for (auto &file : filepathStrings)
    //     {
    //         std::smatch match;
    //         std::regex_search(file, match, re);
    //         group_options.insert(match[i + 1]);
    //     }
    //     options.push_back(group_options);
    // }

    // display options
    for (auto &option : options)
    {
        std::cout << "option.size(): " << option.size() << std::endl;
        for (auto &item : option)
            std::cout << "    item: " << item << std::endl;
    }

    return 0;
}