

import pyparsing as pp

enumCode = """
enum myenum {
	RED  78,
	BLUE,
	PINK
}

"""
LBRACE, RBRACE,  COMMA = map(pp.Suppress, "{},")
identifier = pp.Word(pp.alphas, pp.alphanums + "_")
integer = pp.Word(pp.nums)
enumValue = pp.Group(identifier("name") + pp.Optional(integer("value")))
enumList = pp.Group(enumValue + pp.ZeroOrMore(COMMA + enumValue))
enum = identifier("enum") + LBRACE + enumList("names") + RBRACE

# find instances of enums ignoring other syntax
for item, start, stop in enum.scanString(enumCode):
	print(item)


functionCode = """

void fun() {
	body;
}
"""