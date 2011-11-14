
#ifndef TRE_DETAIL_LIVE_CONFIG_READER_H_
#define TRE_DETAIL_LIVE_CONFIG_READER_H_

#include <string>
#include <vector>

namespace tre {
namespace detail {

    /**
     * Reads live.cfg file and provides information on the
     * type of archive indexing (tre or toc) and the list of
     * index files in the order they should be searched in.
     */
    class LiveConfigReader
    {
    public:
        explicit LiveConfigReader(const std::string& live_file);

        const std::string& GetIndexType() const;
        const std::vector<std::string>& GetIndexFiles() const;

    private:
        std::string index_type_;
        std::vector<std::string> index_files_;
    };

}}  // namespace tre::detail

#endif  // TRE_DETAIL_LIVE_CONFIG_READER_H_
