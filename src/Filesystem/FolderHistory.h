#pragma once
#include <string>
#include <deque>
#include <fstream>
const std::string history_file = "folderhistory.txt";

class FolderHistory
{
  public:
    FolderHistory() { read(); }

    std::string getLatestFolder()
    {
        if (m_FolderHistory.size() > 0) { return m_FolderHistory.front(); }
        return std::string();
    }

    void addFolder(const std::string &folder)
    {
        // check if the folder is already in the history
        auto it = std::find(m_FolderHistory.begin(), m_FolderHistory.end(), folder);
        if (it != m_FolderHistory.end())
        {
            // if it is, move it to the front of the deque
            m_FolderHistory.erase(it);
        }
        // add the folder to the history
        m_FolderHistory.push_front(folder);
        // store the history
        store();
    }

    void removeFolder(const std::string &folder)
    {
        // check if the folder is in the history
        auto it = std::find(m_FolderHistory.begin(), m_FolderHistory.end(), folder);
        if (it != m_FolderHistory.end())
        {
            // if it is, remove it
            m_FolderHistory.erase(it);
            // store the history
            store();
        }
    }

    std::deque<std::string> getHistory() { return m_FolderHistory; }
    void clearHistory() { clear(); }

  private:
    void read()
    {
        // read from file and put every line in the vector
        std::ifstream file(history_file);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                m_FolderHistory.push_back(line);
            }
            file.close();
        }
    }

    void store()
    {
        // write the vector to the file
        std::ofstream file(history_file);
        if (file.is_open())
        {
            for (auto &line : m_FolderHistory)
            {
                file << line << std::endl;
            }
            file.close();
        }
    }

    void clear()
    {
        m_FolderHistory.clear();
        store();
    }

  private:
    std::deque<std::string> m_FolderHistory;
};