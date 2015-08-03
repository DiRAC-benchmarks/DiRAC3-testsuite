 program UMTmk

!use mpi

use kind_mod
use constant_mod
use size_mod
use geometry_mod
use zonedata_mod


 implicit none

!type(MeshSize) :: Size
!type(Geometry) :: Geom
double precision        :: omp_get_wtime
double precision        :: t0, t1, del_t
integer                 :: ZoneID
integer                 :: Angle, Groups
real (adqt)             :: omega (3)
real (adqt)             :: chk
integer, allocatable    :: next(:) ! next(Size%ncornr+1)
integer, allocatable    :: nextZ(:) ! nextZ(Size%nzones)
real(adqt), allocatable :: psic(:,:) ! psic(Size%ngr,Size%ncornr), 
real(adqt), allocatable :: psib(:,:) ! psib(Size%ngr,Size%nbelem)
type(ZoneData), pointer :: pZ   ! abbrev for ZData(ZoneID)

 integer n, myid, numprocs, i, ierr

 !call MPI_INIT(ierr)
 !call MPI_COMM_RANK(MPI_COMM_WORLD, myid, ierr)
 !call MPI_COMM_SIZE(MPI_COMM_WORLD, numprocs, ierr)


 write(*,*)
 write(*,*) 'UMT1.2 Microkernel Benchmark (single CPU) '
 write(*,*)

call constructSize (Size)
call getSize (Size)
!call constructGeom (Geom)
allocate (Geom)
write (6,*) " Geom was allocated"

allocate ( Geom% ZData(Size% nzones) )

do ZoneID = 1, Size%nzones
    pZ => Geom%ZData(ZoneID)
    pZ% nCorner = 8
    pZ%nCFaces = 3
    pZ%nFaces = 6
    allocate (pZ % Connect (3, pZ%nCFaces, pZ%nCorner) )
    allocate( pZ % VolumeOld(pZ% nCorner) )
    allocate( pZ % Volume(pZ% nCorner) )
    allocate( pZ % Sigt(Size% ngr) )
    allocate( pZ % SigtInv(Size% ngr) )
    allocate( pZ % A_fp(Size% ndim,pZ% nCFaces,pZ% nCorner) )
    allocate( pZ % A_ez(Size% ndim,pZ% nCFaces,pZ% nCorner) )
    allocate( pZ % Connect(3,pZ% nCFaces,pZ% nCorner) )
    allocate( pZ % STotal(Size% ngr,pZ% nCorner) )
    allocate( pZ % STime(Size% ngr,pZ% nCorner,Size% nangSN) )
!   if (mod (ZoneID, 1000) == 0 ) write (6,*) ' Allocated up to ZoneID ', ZoneID
enddo
 write (6,*) ' Allocated pZ % Connect (3, pZ%nCFaces, pZ%nCorner) '

do ZoneID = 1, Size%nzones
  pZ => Geom%ZData(ZoneID)
     pZ%Volume(:) = 1.0          ! corner volume
     pZ%Sigt(:) = 1.11111        ! total opacity
     pZ%SigtInv(:) = 0.9         ! reciprocal of total opacity
     pZ%STotal(:,:) = .001       ! fixed + scattering source
     pZ%STime(:,:,:) = .001      ! time-dependent source

     pZ%A_fp(:,:,:) = 1.1        ! outward normals on corner faces
     pZ%A_ez(:,:,:) = 1.2        !
!   if (mod (ZoneID, 1000) == 0 ) write (6,*) ' Allocated up to ZoneID ', ZoneID
enddo

 write (6,*) ' Initialized pZ%A_fp(:,:,:) pZ%A_ez(:,:,:)  '

!   write(6,*) 'Checking pZ % A_fp(Size% ndim,pZ% nCFaces,pZ% nCorner) ', pZ%A_fp(Size% ndim,pZ% nCFaces,pZ% nCorner) 
!   write(6,*) 'Checking pZ % A_ez(Size% ndim,pZ% nCFaces,pZ% nCorner) ', pZ%A_ez(Size% ndim,pZ% nCFaces,pZ% nCorner)

do ZoneID = 1, Size%nzones
  pZ => Geom%ZData(ZoneID)
  read (10,*) pZ%c0, pZ%Connect(1:3,1:pZ%nCFaces,1:pZ%nCorner )
enddo
  write (6,*) ' Initialized pZ%c0, pZ%Connect(1:3,1:pZ%nCFaces,1:pZ%nCorner ) for all zones '



omega (1) =  0.980500879011739      
omega (2) = -0.138956875067780      
omega (3) = -0.138956875067780     

write (6,*) ' Done omega (3) '


Groups = Size%ngr 
Angle = 253
!
! Allocate and initialize next(Size%ncornr+1)
!

allocate ( next(Size%ncornr+1)  )
read (14,*) next(1:Size%ncornr+1)
write (6,*) ' Done allocating and initializing next(Size%ncornr+1) '

!
! Allocate and initialize nextZ(Size%nzones)
!
allocate ( nextZ(Size%nzones) )
read (13,*) nextZ(1:Size%nzones) 
write (6,*) ' Done allocating and initializing nextZ(1:Size%nzones) '


allocate ( psic(Size%ngr,Size%ncornr) )
allocate (  psib(Size%ngr,Size%nbelem) )

do i = 1, Groups
     psib (i,:) = i*1.0e-10
     psic (i,:) = i*1.0e-10
enddo

!
! BEGIN COMPUTE INTENSIVE SECTION
!

t0 = omp_get_wtime()
call snswp3d (Groups, Angle, next, nextZ, psic, psib, omega, chk)    
t1 = omp_get_wtime()
del_t = t1 - t0
!
! END   COMPUTE INTENSIVE SECTION
!
write (6,*) ' V&V chk = ', chk

 write (6,*) "UMT1.2 microkernel Walltime (second) ", del_t

! call MPI_FINALIZE(ierr)

10 format(// "TOTAL WALLTIME IME ", e20.5 /)

 stop
 end

 
!!!!!!!!!!!!!!!!!!!!!!
subroutine getSize (self)
!!!!!!!!!!!!!!!!!!!!!!
use Size_mod
type (MeshSize), intent (in) :: self
write (6,*) "Enter subroutine getSize "
 
write (6,*) "nzones= ", self%nzones  
write (6,*) "ncornr= ", self%ncornr  
write (6,*) "nfaces= ", self%nfaces  
write (6,*) "nbelem= ", self%nbelem   
write (6,*) "ndim  = ", self%ndim     
write (6,*) "nangsn= ", self%nangsn   
write (6,*) "ngr   = ", self%ngr      

write (6,*) "Exit  subroutine getSize "
return 
end subroutine 
