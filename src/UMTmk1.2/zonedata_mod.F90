! ZoneData Module:  Contains data structures for zone information 

module ZoneData_mod 

  use kind_mod
  use constant_mod

  private

! public interfaces

  public constructZone
                                                                                 
  type, public :: ZoneData 

     integer              :: nCorner            ! number of corners
     integer              :: nCFaces            ! number of corner faces 
     integer              :: c0                 ! global corner ID of first corner
     integer              :: nFaces             ! number of zone faces
     real(adqt)           :: VolumeZone         ! zone volume
     real(adqt)           :: EnergyDensityOld   ! old energy density

     real(adqt), pointer  :: Area(:)            ! corner area
     real(adqt), pointer  :: VolumeOld(:)       ! old corner volume
     real(adqt), pointer  :: Volume(:)          ! corner volume
     real(adqt), pointer  :: Sigt(:)            ! total opacity
     real(adqt), pointer  :: SigtInv(:)         ! reciprocal of total opacity
     real(adqt), pointer  :: STotal(:,:)        ! fixed + scattering source
     real(adqt), pointer  :: STime(:,:,:)       ! time-dependent source

     real(adqt), pointer  :: A_fp(:,:,:)        ! outward normals on corner faces 
     real(adqt), pointer  :: A_ez(:,:,:)        !
     real(adqt), pointer  :: Radius(:,:,:)        ! average radius
     integer,    pointer  :: Connect(:,:,:)       ! nearest neighbor connectivity 

  end type ZoneData 

  type(ZoneData), pointer, public :: ZZ

  interface constructZone
    module procedure ZoneData_ctor
  end interface

!  interface destruct
!    module procedure ZoneData_dtor
!  end interface

contains

!=======================================================================
! construct interface
!=======================================================================
                                                                                   
   subroutine ZoneData_ctor(self,        &
                            nCorner,     &
                            nCFaces,     &
                            corner0,     &
                            nFaces,      &
                            Connect)

!   subroutine ZoneData_ctor (self)

    use Size_mod

    implicit none

!   Passed variables

    type(ZoneData), intent(inout)    :: self

    integer, intent(in)              :: nCorner
    integer, intent(in)              :: nCFaces       
    integer, intent(in)              :: corner0
    integer, intent(in)              :: nFaces
    integer, intent(in)              :: Connect(3,Size%maxcf,Size%maxCorner) 

!   Local

    integer          :: cID, i 

!   Set Properties

!   self% nCorner = Size% nCorner 
!   self% nCFaces = nCFaces
!   self% c0      = corner0
!   self% nFaces  = nFaces
    self% nCorner = 8
    self% nCFaces = 3
!   self% c0      = corner0
    self% nFaces  = 6      

    allocate( self % VolumeOld(self% nCorner) )
    allocate( self % Volume(self% nCorner) )
    allocate( self % Sigt(Size% ngr) )
    allocate( self % SigtInv(Size% ngr) )
    allocate( self % A_fp(Size% ndim,self% nCFaces,self% nCorner) )
    allocate( self % A_ez(Size% ndim,self% nCFaces,self% nCorner) )
    allocate( self % Connect(3,self% nCFaces,self% nCorner) )
    allocate( self % STotal(Size% ngr,self% nCorner) )
    allocate( self % STime(Size% ngr,self% nCorner,Size% nangSN) )

!   if (Size%ndim == 2) then
!     allocate( self % Area(self% nCorner) )
!     allocate( self % Radius(2,2,self% nCorner) )
!   endif

!   do cID=1,self% nCorner
!     do i=1,self% nCFaces
!       self % Connect(1,i,cID) = Connect(1,i,cID)
!       self % Connect(2,i,cID) = Connect(2,i,cID)
!       self % Connect(3,i,cID) = Connect(3,i,cID) - corner0
!     enddo
!   enddo


    return

  end subroutine ZoneData_ctor

!=======================================================================
! destruct interface
!=======================================================================
                                                                                    
!  subroutine ZoneData_dtor(self, Ndim)


!    implicit none

!   Passed variables
                                                                                     

!    return

!  end subroutine ZoneData_dtor


end module ZoneData_mod

