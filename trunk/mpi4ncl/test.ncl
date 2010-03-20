load "$NCARG_ROOT/lib/ncarg/nclscripts/mpi/mpi4ncl.ncl"

begin
    ; Init, args needs to contain the complete command line as one string
    err = MPI_Init(args)
    print("MPI_Init: " + err)

    ; get the rank
    rank = 0
    err = MPI_Comm_rank(MPI_COMM_WORLD, rank)
    print("MPI_Comm_rank: " + rank)

    ; get the number of processes
    size = 0
    err = MPI_Comm_size(MPI_COMM_WORLD, size)
    print("MPI_Comm_size: " + size)

    ; Rank 0 send data to rank 1
    if (rank .eq. 0) then
        daten = (/(/40.3,28.5,38.2,23.0,23.5,123.3/),(/1.3,23.4,12.4,0.4,1.24,2.3/)/)
        err = MPI_Send(daten, 1, 0, MPI_COMM_WORLD)
        print("MPI_Send: " + err)
        print(daten)
    end if
    if (rank .eq. 1) then
        daten = new((/2,6/),float)
        err = MPI_Recv(daten, 0, 0, MPI_COMM_WORLD)
        print("MPI_Recv: " + err)
        print(daten)
    end if

    err = MPI_Finalize()
    print("MPI_Finalize: " + err)
    exit
end
