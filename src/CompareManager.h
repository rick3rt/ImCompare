#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <regex>
#include <set>

#include "Log.h"
#include "FileManager.h"

class RegexComparer
{
  public:
    RegexComparer() {}
    ~RegexComparer() {}

    void SetRegexString(const std::string &regex)
    {
        m_RegexString = regex;
        Compile();
    }

    bool Compile()
    {
        try
        {
            m_Regex = std::regex(m_RegexString);
            m_RegexValid = true;
        }
        catch (const std::regex_error &e)
        {
            LOG_ERROR("Regex error: {}", e.what());
            m_RegexValid = false;
        }
        return m_RegexValid;
    }
    void SetFiles(const std::vector<std::string> &files) { m_Files = files; }
    const std::vector<std::set<std::string>> &GetOptions() const { return m_Options; }
    const bool &GetRegexValid() const { return m_RegexValid; }

    const std::string &GetRegexString() const { return m_RegexString; }
    std::string *GetRegexString() { return &m_RegexString; }

    void Compute()
    {
        // the regular expression to test
        size_t num_groups = m_Regex.mark_count();
        // debug logging
        LOG_DEBUG("num_groups: {}", num_groups);
        for (auto &file : m_Files)
            LOG_DEBUG("file: {}", file);

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

    void PrintOptions()
    {
        for (auto &option : m_Options)
        {
            LOG_INFO("option.size(): {}", option.size());
            for (auto &item : option)
                LOG_INFO("    item: {}", item);
        }
    }

  private:
    std::string m_RegexString;
    std::regex m_Regex;
    bool m_RegexValid = false;
    std::vector<std::set<std::string>> m_Options;
    std::vector<std::string> m_Files;
};

class CompareManager
{
  public:
    CompareManager();
    ~CompareManager();

    void SetBasePath(std::filesystem::path path);

    void Update();

  private:
    FileManager m_FileManager;
    RegexComparer m_RegexComparer;

    std::string m_BasePath;
    std::string m_Regex;
    std::vector<std::string> m_FolderContents;
};
