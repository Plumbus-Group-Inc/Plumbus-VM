#include <sstream>

#include "common/value.hpp"

namespace pvm {

ValueMismatchError::ValueMismatchError(std::type_info const& requestedType, 
        std::type_info const& currentType) : 
    std::runtime_error(formatMessage(requestedType, currentType)),
    requestedType(requestedType), currentType(currentType) {}

std::string ValueMismatchError::formatMessage(std::type_info const& requetedType,
    std::type_info const& currentType) {
    std::stringstream s;
    s << "Value has type " << currentType.name() << 
        ", but attempted to modify " << requetedType.name() << " type";

    return s.str();
}

} // namespace pvm
