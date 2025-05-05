#include "bc.hh"
#include <parser.hh>

using namespace bc;

int main() {
	BC bc = BC("b");
	bc.input = R"(
main(){
	auto x, y;
	x = 23;
	y = 3;
	return x * y;
}
)";

	auto result = bc.parseAndCompile();
	if (result.is_error()) {
		fmt::println("[bc] {}", result.get_error().value());
		return 1;
	}

	bc.emit("a.out", BC::EMIT_IR);
}
