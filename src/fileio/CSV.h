
#pragma once

#include <istream>
#include <string>
#include <vector>
#include <initializer_list>

namespace fileio
{

class CSVCell;
class CSVRow;
class CSVTable;

class CSVCell
{
    friend CSVRow;

public:
    explicit CSVCell() = default;
    CSVCell(const std::string&);

    inline auto row() const { return m_Row; }
    inline auto col() const { return m_Col; }
    inline auto str() const -> const std::string& { return m_String; }

protected:
    size_t m_Row{};
    size_t m_Col{};
    std::string m_String{};
};

class CSVRow : public std::vector<CSVCell>
{
    friend CSVTable;

public:
    explicit CSVRow() = default;
    CSVRow(const std::initializer_list<CSVCell>&);

    inline auto row() const { return m_Row; }
    auto ncols() const -> size_t;

    auto str() const -> std::string;

protected:
    size_t m_Row{};
};

class CSVTable : public std::vector<CSVRow>
{
public:
    explicit CSVTable() = default;
    CSVTable(const std::initializer_list<CSVRow>&);

    auto nrows() const -> size_t;
    auto ncols() const -> size_t;

    auto str() const -> std::string;
};

namespace CSVReader
{
    auto ReadCSVTable(std::istream&, char delim)
        -> CSVTable;
    auto ReadCSVTable(const std::string&, char delim)
        -> CSVTable;
}

} // namespace fileio
