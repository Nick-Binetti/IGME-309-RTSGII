#include <iostream>

class NumberProcessor // The Caller
{
public:
    NumberProcessor() : oddCallback(nullptr), evenCallback(nullptr) {}

    // Register callbacks
    void setOddCallback(void (*cb)(int))
    {
        oddCallback = cb;
    }

    void setEvenCallback(void (*cb)(int))
    {
        evenCallback = cb;
    }

    // Process numbers
    void process(int arr[], int size) // The Event Loop
    {
        for (int i = 0; i < size; i++)
        {
            if (arr[i] % 2 == 0)
            {
                if (evenCallback)
                    evenCallback(arr[i]);
            }
            else
            {
                if (oddCallback)
                    oddCallback(arr[i]);
            }
        }
    }

private:
    // Defining a function pointer:
    // return_type (*pointer_name)(parameter_types);
    void (*oddCallback)(int);  // function pointer for odd numbers
    void (*evenCallback)(int); // function pointer for even numbers
};

// Callback functions
void onOdd(int n) // The Callback
{
    std::cout << "Odd number: " << n << std::endl;
}

void onEven(int n) // The Callback
{
    std::cout << "Even number: " << n << std::endl;
}

int main()
{
    int numbers[] = {1, 2, 3, 4, 5, 6};

    NumberProcessor processor;
    processor.setOddCallback(onOdd);   // Registration
    processor.setEvenCallback(onEven); // Registration

    processor.process(numbers, 6);

    return 0;
}
