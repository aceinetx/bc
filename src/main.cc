#include "bc.hh"
#include <parser.hh>

using namespace bc;

int main() {
	BC bc = BC("b");
	bc.input = R"(
f(a, b){
	return (a * b) + (a + b);
}

main(){
	return f(23, 3, 4);
}
)";

	auto result = bc.parseAndCompile();
	if (result.is_error()) {
		fmt::println("[bc] {}", result.get_error().value());
		return 1;
	}

	bc.emit("a.out", BC::EMIT_IR);
}
