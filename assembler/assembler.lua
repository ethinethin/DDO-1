function gencom(command, indirect, page, operand)
        binary = 0
        -- opcode
        if command == "AND" then
                binary = binary + tonumber("0000000000000000", 2)
        elseif command == "TAD" then
                binary = binary + tonumber("0010000000000000", 2)
        elseif command == "ISZ" then
                binary = binary + tonumber("0100000000000000", 2)
        elseif command == "DCA" then
                binary = binary + tonumber("0110000000000000", 2)
        elseif command == "JMS" then
                binary = binary + tonumber("1000000000000000", 2)
        elseif command == "JMP" then
                binary = binary + tonumber("1010000000000000", 2)
        end
        -- indirect bit
        if indirect == 0 then
                binary = binary + tonumber("0000000000000000", 2)
        else
                binary = binary + tonumber("0001000000000000", 2)
        end
        -- page bit
        if page == 0 then
                binary = binary + tonumber("0000000000000000", 2)
        else
                binary = binary + tonumber("0000100000000000", 2)
        end
        -- operand
        binary = binary + operand
        print(binary)
end
