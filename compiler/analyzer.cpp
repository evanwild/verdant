#include "analyzer.h"

#include <cctype>
#include <format>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include "parser.h"

void analyze_tree(const ProgramNode &tree) {
    std::unordered_set<std::string> component_names;

    for (const auto &component : tree.components) {
        if (!std::isupper(component.name[0])) {
            const auto error_msg = std::format(
                "Analyzing error: The component name \"{}\" must start with an "
                "uppercase letter",
                component.name);
            throw std::runtime_error(error_msg);
        }

        if (component_names.contains(component.name)) {
            const auto error_msg = std::format(
                "Analyzing error: There are two components named \"{}\"",
                component.name);
            throw std::runtime_error(error_msg);
        }

        component_names.insert(component.name);
    }

    if (!component_names.contains("Main")) {
        throw std::runtime_error(
            "Analyzing error: There must be a component named \"Main\"");
    }
}
