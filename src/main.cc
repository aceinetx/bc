#include "bc.hh"
#include <parser.hh>

using namespace bc;

int main() {
	BC bc = BC("b");
	bc.input = R"(
main(){
	auto i, j;
	i = j = 5;
	return j;
}
)";

	auto result = bc.parseAndCompile();
	if (result.is_error()) {
		fmt::println("[bc] {}", result.get_error().value());
		return 1;
	}

	bc.emit("a.out", BC::EMIT_IR);
}
