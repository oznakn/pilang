package main

import (
	"strings"
)

func RunLineRunner(scope *Scope, line string) {
	line = strings.TrimSpace(line)

	if line != "" {
		if strings.Count(line, "=") == 1 {
			parts := strings.Split(line, "=")

			variableName := strings.TrimSpace(parts[0])

			if !IsValidVariableName(variableName) {
				panic("Invalid Variable Name")
			}

			value := ParseValue(scope, parts[1])

			variable := CreateVariable(variableName, value)

			scope.AddVariable(variable)
		} else if strings.Count(line, "(") == 1 && strings.Count(line, ")") == 1 {
			firstIndex := strings.Index(line, "(")

			functionName := string(line[0:firstIndex])

			argumentStrings := strings.Split(line[firstIndex+1:len(line)-1], ",")
			arguments := make([]*Value, len(argumentStrings))

			for key, argumentString := range argumentStrings {
				argumentString = strings.TrimSpace(argumentString)

				arguments[key] = ParseValue(scope, argumentString)
			}

			if scope.IsFunctionExists(functionName) {
				scope.FindFunction(functionName).Run(arguments)
			} else if IsSystemFunctionExists("", functionName) {
				RunSystemFunction("", functionName, arguments)
			}
		}
	}

}
