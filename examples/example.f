program factorial_calculator
    implicit none

! Declare variables
    integer :: num, result

! Ask the user for input
    print *, "Enter a positive integer: "
    read *, num

! Calculate the factorial
    result = factorial(num)

! Print the result
    print *, "The factorial of ", num, " is ", result

    contains

! Function to calculate factorial
    recursive function factorial(n) result(fact)
        integer, intent(in) :: n
        integer :: fact

        if (n == 0) then
            fact = 1
        else
            fact = n * factorial(n - 1)
        end if

    end function factorial

end program factorial_calculator
