#include <mpi.h>
#include <iostream>
#include <string>

// Вывести на экран сообщение из каждого потока по отдельности. 
// по приколу сделал обмен сообщениями между процессами

int main(int argc, char** argv) 
{
    MPI_Init(&argc, &argv); // инициализируется среда mpi

    int number, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &number); // тут определяется номер текущего процесса внутри коммуникатора MPI_COMM_WORLD
    MPI_Comm_size(MPI_COMM_WORLD, &size); // аналогично, но для общего кол-ва процессов

    std::string message = "Privet from process number " + std::to_string(number);

    for (int i = 0; i < size; ++i) // цикл для отправки сообщ. от 1ого процесса к всем остальным
    {
        if (i != number) // проверка на то, что i не равно номеру текущего процесса 
            MPI_Send(message.c_str(), message.size() + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD); //отправка сообщения, но преобразовываем string в c-style строку + задаем размер сообщения + 1, 
                                                                                           // чтобы учитывать \0 символ
    }

    for (int i = 0; i < size; ++i) // аналогичная операция, но с MPI_Recv
    {
        if (i != number) 
        {
            char recv_message[256];
            MPI_Recv(recv_message, 256, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "" << number << " <- \"" << recv_message << "\"\n";
        }
    }

    MPI_Finalize(); // конец работы

    return 0;
}
