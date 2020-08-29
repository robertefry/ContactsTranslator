
#include <istream>
#include <string>
#include <vector>

namespace fileio
{

using CSVCell  = std::string;
using CSVRow   = std::vector<CSVCell>;
using CSVTable = std::vector<CSVRow>;

namespace CSVReader
{
    auto ReadTable(std::istream&, char delim)
        -> CSVTable;
}

} // namespace fileio
