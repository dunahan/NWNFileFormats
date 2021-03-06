#include "FileFormats/2da/2da_Friendly.hpp"
#include "Utility/Assert.hpp"

namespace FileFormats::TwoDA::Friendly {

TwoDARow::TwoDARow(std::vector<TwoDAEntry>&& data,
    std::unordered_map<std::string, std::size_t> const& columns)
    : m_ColumnNames(columns),
      m_Data(std::forward<std::vector<TwoDAEntry>>(data))
{
}

TwoDAEntry const& TwoDARow::operator[](std::size_t column) const
{
    ASSERT(column < m_Data.size());
    return m_Data[column];
}

TwoDAEntry const& TwoDARow::operator[](std::string const& column) const
{
    auto columnName = m_ColumnNames.find(column);
    ASSERT(columnName != std::end(m_ColumnNames));
    return m_Data[columnName->second];
}

std::string const& TwoDARow::AsStr(std::size_t column) const
{
    return operator[](column).m_Data;
}

std::string const& TwoDARow::AsStr(std::string const& column) const
{
    return operator[](column).m_Data;
}

std::int32_t TwoDARow::AsInt(std::size_t column) const
{
    return atoi(operator[](column).m_Data.c_str());
}

std::int32_t TwoDARow::AsInt(std::string const& column) const
{
    return atoi(operator[](column).m_Data.c_str());
}

float TwoDARow::AsFloat(std::size_t column) const
{
    return static_cast<float>(atof(operator[](column).m_Data.c_str()));
}

float TwoDARow::AsFloat(std::string const& column) const
{
    return static_cast<float>(atof(operator[](column).m_Data.c_str()));
}

TwoDARow::TwoDAEntries::const_iterator TwoDARow::begin() const
{
    return std::cbegin(m_Data);
}

TwoDARow::TwoDAEntries::const_iterator TwoDARow::end() const
{
    return std::cend(m_Data);
}

std::size_t TwoDARow::size() const
{
    return m_Data.size();
}

TwoDA::TwoDA(Raw::TwoDA const& raw2da)
{
    // Line 1 we don't care about ...
    // Line 2 has default values. TODO
    // Line 3 has all the columns.
    ASSERT(raw2da.m_Lines.size() >= 3);

    // Iterate over all of the column names and set up the map.
    for (std::size_t i = 0; i < raw2da.m_Lines[2].m_Tokens.size(); ++i)
    {
        std::string const& token = raw2da.m_Lines[2].m_Tokens[i];
        m_ColumnNames[token] = i;
    }

    // Iterate over all of the entries and set them up.
    for (std::size_t i = 3; i < raw2da.m_Lines.size(); ++i)
    {
        std::vector<TwoDAEntry> entries;
        std::vector<Raw::TwoDAToken> const& tokens = raw2da.m_Lines[i].m_Tokens;

        // Skip the first token (which is the row number) when setting this up.
        for (std::size_t j = 1; j < tokens.size(); ++j)
        {
            Raw::TwoDAToken const& token = tokens[j];

            TwoDAEntry entry;

            if (token == "****")
            {
                entry.m_IsDefault = true;
            }
            else
            {
                entry.m_Data = token;
                entry.m_IsDefault = false;
            }

            entries.emplace_back(std::move(entry));

        }

        m_Rows.emplace_back(std::move(entries), m_ColumnNames);
    }
}

std::string const& TwoDA::AsStr(std::size_t row, std::size_t column) const
{
    return m_Rows[row].AsStr(column);
}

std::string const& TwoDA::AsStr(std::size_t row, std::string const& column) const
{
    return m_Rows[row].AsStr(column);
}

std::int32_t TwoDA::AsInt(std::size_t row, std::size_t column) const
{
    return m_Rows[row].AsInt(column);
}

std::int32_t TwoDA::AsInt(std::size_t row, std::string const& column) const
{
    return m_Rows[row].AsInt(column);
}

float TwoDA::AsFloat(std::size_t row, std::size_t column) const
{
    return m_Rows[row].AsFloat(column);
}

float TwoDA::AsFloat(std::size_t row, std::string const& column) const
{
    return m_Rows[row].AsFloat(column);
}

TwoDARow const& TwoDA::operator[](std::size_t i) const
{
    ASSERT(i < m_Rows.size());
    return m_Rows[i];
}

TwoDA::TwoDARows::const_iterator TwoDA::begin() const
{
    return std::cbegin(m_Rows);
}

TwoDA::TwoDARows::const_iterator TwoDA::end() const
{
    return std::cend(m_Rows);
}

std::size_t TwoDA::size() const
{
    return m_Rows.size();
}

std::unordered_map<std::string, std::size_t> const& TwoDA::GetColumnNames() const
{
    return m_ColumnNames;
}

}
