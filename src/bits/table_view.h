
#pragma once

#include <vector>
#include <initializer_list>
#include <functional>
#include <string>
#include <sstream>
#include <algorithm>

#include "util/string.h"

/******************************************************************************/

namespace bits
{

    template <typename _Tp>
    class TableView
    {
    public:
        explicit TableView() = default;

        template <typename ForwardIterator>
        TableView(size_t nrows, size_t ncols, ForwardIterator itr);

        TableView(size_t nrows, size_t ncols, const std::vector<_Tp>&);
        TableView(size_t nrows, size_t ncols, const std::vector<_Tp*>&);
        TableView(const std::vector<std::vector<_Tp>>&);
        TableView(const std::vector<std::vector<_Tp*>>&);

        TableView(const TableView&);
        TableView(TableView&&);
        TableView& operator=(const TableView&);
        TableView& operator=(TableView&&);

        inline const auto table() const { return m_TableView; }
        inline auto nrows() const { return m_NumRows; }
        inline auto ncols() const { return m_NumCols; }

        template <typename ToString>
        auto str(ToString to_string) const -> std::string;

    protected:
        void recache();

        std::vector<std::vector<const _Tp*>> m_TableView{};
        size_t m_NumRows{};
        size_t m_NumCols{};
    };

}

/******************************************************************************/

template <typename _Tp>
template <typename ForwardIterator2D>
bits::TableView<_Tp>::TableView(size_t nrows, size_t ncols, ForwardIterator2D row_itr)
    : m_TableView{}
    , m_NumRows{nrows}
    , m_NumCols{ncols}
{
    m_TableView.resize(m_NumRows);
    for (size_t i = 0; i < m_NumRows; ++i) {
        m_TableView[i].resize(m_NumCols);
        auto cell_itr = row_itr++;
        for (size_t j = 0; j < m_NumCols; ++j) {
            m_TableView[i][j] = &*cell_itr++;
        }
    }
}

template <typename _Tp>
bits::TableView<_Tp>::TableView(size_t nrows, size_t ncols, const std::vector<_Tp>& vec)
    : m_TableView{}
    , m_NumRows{nrows}
    , m_NumCols{ncols}
{
    static_assert(vec.size() >= nrows*ncols);
    m_TableView.resize(m_NumRows);
    for (size_t i = 0; i < m_NumRows; ++i) {
        m_TableView[i].resize(m_NumCols);
        for (size_t j = 0; j < m_NumCols; ++j) {
            m_TableView[i][j] = &vec[i * m_NumCols + j];
        }
    }
}

template <typename _Tp>
bits::TableView<_Tp>::TableView(size_t nrows, size_t ncols, const std::vector<_Tp*>& vec)
    : m_TableView{}
    , m_NumRows{nrows}
    , m_NumCols{ncols}
{
    static_assert(vec.size() >= nrows*ncols);
    m_TableView.resize(m_NumRows);
    for (size_t i = 0; i < m_NumRows; ++i) {
        m_TableView[i].resize(m_NumCols);
        for (size_t j = 0; j < m_NumCols; ++j) {
            m_TableView[i][j] = vec[i * m_NumCols + j];
        }
    }
}

template <typename _Tp>
bits::TableView<_Tp>::TableView(const std::vector<std::vector<_Tp>>& vec)
    : m_TableView{}
    , m_NumRows{vec.size()}
    , m_NumCols{vec.empty() ? 0 : vec[0].size()}
{
    m_TableView.resize(m_NumRows);
    for (size_t i = 0; i < m_NumRows; ++i) {
        m_TableView[i].resize(m_NumCols);
        for (size_t j = 0; j < m_NumCols; ++j) {
            m_TableView[i][j] = &vec[i][j];
        }
    }
}

template <typename _Tp>
bits::TableView<_Tp>::TableView(const std::vector<std::vector<_Tp*>>& vec)
    : m_TableView{vec}
    , m_NumRows{vec.size()}
    , m_NumCols{vec.empty() ? 0 : vec[0].size()}
{
}

template <typename _Tp>
bits::TableView<_Tp>::TableView(const TableView& other)
    : m_TableView{other.m_TableView}
    , m_NumRows{other.m_NumRows}
    , m_NumCols{other.m_NumCols}
{
}

template <typename _Tp>
bits::TableView<_Tp>::TableView(TableView&& other)
    : m_TableView{std::move(other.m_TableView)}
{
    recache();
}

template <typename _Tp>
bits::TableView<_Tp>& bits::TableView<_Tp>::operator=(const TableView& other)
{
    m_TableView = other.m_TableView;
    m_NumRows = other.m_NumRows;
    m_NumCols = other.m_NumCols;
}

template <typename _Tp>
bits::TableView<_Tp>& bits::TableView<_Tp>::operator=(TableView&& other)
{
    m_TableView = std::move(other.m_TableView);
    recache();
}

template <typename _Tp>
void bits::TableView<_Tp>::recache()
{
    m_NumRows = m_TableView.size();
    m_NumCols = std::max_element(m_TableView.cbegin(), m_TableView.cend(),
        [](const auto& a, const auto& b) { return a.size() < b.size(); } )->size();

    std::for_each(m_TableView.begin(), m_TableView.end(), [this](auto& row) {
        row.resize(m_NumCols);
    });
}

template <typename _Tp>
template <typename ToString>
auto bits::TableView<_Tp>::str(ToString to_string) const -> std::string
{
    if (m_NumRows == 0) return "╭───╮\n╰───╯";

    const auto make_string = [&](const auto* item) -> std::string {
        return (item == nullptr) ? "" : to_string(*item);
    };

    // Calculate table of strings to print
    std::vector<std::vector<std::string>> string_table;
    string_table.reserve(m_NumRows);
    for (size_t i = 0; i < m_NumRows; ++i) {
        std::vector<std::string> st_row;
        st_row.reserve(m_NumCols);
        for (size_t j = 0; j < m_NumCols; ++j) {
            auto string = make_string(m_TableView[i][j]);
            st_row.push_back(std::move(string));
        }
        string_table.push_back(std::move(st_row));
    }

    // Calculate the widths of each column
    std::vector<size_t> widths(m_NumCols);
    for (size_t j = 0; j < m_NumCols; ++j) {
        size_t width_max = 0;
        for (size_t i = 0; i < m_NumRows; ++i) {
            width_max = std::max(width_max, string_table[i][j].size());
        }
        widths[j] = width_max;
    }

    std::ostringstream ss;
    // top border
    ss << "╭─" << util::repeat_string("─", widths[0]);
    for (size_t j = 1; j < m_NumCols; ++j) {
        ss << "───" << util::repeat_string("─", widths[j]);
    }
    ss << "─╮" << std::endl;
    // header items
    ss << "│ " << util::pad_string_inplace(string_table[0][0], widths[0]);
    for (size_t j = 1; j < m_NumCols; ++j) {
        ss << " ┊ " << util::pad_string_inplace(string_table[0][j], widths[j]);
    }
    ss <<  " │" << std::endl;
    // header divider
    ss << "│ " << util::repeat_string("┄", widths[0]);
    for (size_t j = 1; j < m_NumCols; ++j) {
        ss << "┄┼┄" << util::repeat_string("┄", widths[j]);
    }
    ss << " │" << std::endl;
    // table entries
    for (size_t i = 1; i < m_NumRows; ++i) {
        ss << "│ " << util::pad_string_inplace(string_table[i][0], widths[0]);
        for (size_t j = 1; j < m_NumCols; ++j) {
            ss << " ┊ " << util::pad_string_inplace(string_table[i][j], widths[j]);
        }
        ss << " │" << std::endl;
    }
    // bottom border
    ss << "╰─" << util::repeat_string("─", widths[0]);
    for (size_t j = 1; j < m_NumCols; ++j) {
        ss << "───" << util::repeat_string("─", widths[j]);
    }
    ss << "─╯";

    return ss.str();
}
