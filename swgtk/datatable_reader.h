
#ifndef SWGTK_DATATABLE_READER_H_
#define SWGTK_DATATABLE_READER_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <boost/any.hpp>

namespace swgtk {
	
    namespace detail {
        /**
         * Attempts to cast the cell's value to the specified type T.
         *
         * \return The cell's value cast as the given type T.
         */
        template<typename T>
        T GetValue(const boost::any& value)
        {
            return *boost::any_cast<const T*>(value);
        }

        /**
         * An overload of GetValue that can turn char* into string.
         *
         * \return The cell's value as a string.
         */
        template<>
        inline std::string GetValue<std::string>(const boost::any& value)
        {
            return std::string(boost::any_cast<const char*>(value));
        }
    }

    class DatatableCell
    {
    public:
        std::string ToString() const;

        template<typename T>
        T GetValue() const
        {            
            return detail::GetValue<T>(value_);
        }

        template<typename T>
        void SetValue(T value)
        {
            value_ = value;
        }

    private:
        boost::any value_;
    };
	
    class DatatableReader 
    {
    public:
        DatatableReader(std::vector<char>&& input);
        
        uint32_t CountRows() const;
        
        const std::vector<std::string>& GetColumnNames() const;

        bool Next();

        std::map<std::string, DatatableCell*> GetRow();

    private:
        void ValidateFile(const std::vector<char>& input) const;

        void ParseColumnNames();
        void ParseColumnTypes();
        void ParseRows();
        uint32_t ParseColumn(uint8_t type, uint32_t offset, std::vector<DatatableCell>& row);

        struct ColumnHeader {
            char name[4];
            uint32_t size;
            uint32_t count;
        };
        
        struct TypeHeader {
            char name[4];
            uint32_t size;
        };

        struct RowHeader {
            char name[4];
            uint32_t size;
            uint32_t count;
        };

        std::vector<char> input_;

        uint32_t current_row_;

        const ColumnHeader* column_header_;
        const char* column_offset_;
        std::vector<std::string> column_names_;

        const TypeHeader* type_header_;
        const char* type_offset_;
        std::vector<std::string> column_types_;

        const RowHeader* row_header_;
        const char* row_offset_;

        std::vector<std::vector<DatatableCell>> row_data_;
    };
    
}  // namespace swgtk

#endif  // SWGTK_DATATABLE_READER_H_
