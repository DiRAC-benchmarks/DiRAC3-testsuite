! Geometry Modules:  geometry and connectivity information used by Sn
                                                                                 
module Geometry_mod 

  use kind_mod
  use ZoneData_mod

  private

! public interfaces
                                                                                             
  public constructGeom, destructGeom, getZoneData
                                                                                 
                                                                                 
  type, public :: Geometry 

     real(adqt), pointer :: px(:,:)           ! px(ndim,npnts)  - coordinates of zone vertices
     real(adqt), pointer :: volc(:)           ! volc(ncornr)    - corner volumes

!    1D & 2D Specific Arrays
     real(adqt), pointer :: areac(:)          ! areac(ncornr)
     real(adqt), pointer :: rj2(:)            ! rj2(nzones)
     real(adqt), pointer :: r2(:)             ! r2(nzones+1)
     real(adqt), pointer :: hj(:)             ! hj(nzones)

     integer, pointer :: CToFace(:,:)         ! CToFace(maxcf,ncornr)
     integer, pointer :: CToZone(:)           ! CToZone(ncornr)
     integer, pointer :: CToPoint(:)          ! CToPoint(ncornr)
     integer, pointer :: ZoneToSrc(:)         ! ZoneToSrc(nzones)
     integer, pointer :: nfpc(:)              ! nfpc(ncornr)

     type(ZoneData), pointer :: ZData(:)      ! zone data pointers

  end type Geometry

  type(Geometry), pointer, public :: Geom

  interface constructGeom
    module procedure Geometry_ctor
  end interface

  interface getZoneData
    module procedure Geometry_getZone
  end interface

  interface destructGeom
    module procedure Geometry_dtor
  end interface

contains

!=======================================================================
! construct interface
!=======================================================================

  subroutine Geometry_ctor(self)

    use Size_mod

    implicit none

!   Passed variables

    type(Geometry),  allocatable, intent(inout) :: self

    integer :: Connect(3,Size%maxcf,Size%maxcorner)
    integer :: c0
    integer :: zoneID
!!$    allocate( self % px(Size%ndim,Size%npnts) )
!kkg    allocate( self % volc(Size%ncornr) )

!   Geometry Specific Arrays

    if (Size%ndim == 1) then
      allocate( self % rj2(Size%nzones) )
      allocate( self % r2(Size%nzones+1) )
      allocate( self % hj(Size%nzones) )
      allocate( self % areac(Size%ncornr) )
    endif

!   Mesh Connectivity

!kkg    allocate( self % CToFace(Size%maxcf,Size%ncornr) )
!kkg    allocate( self % CToZone(Size%ncornr) )
!kkg    allocate( self % CToPoint(Size%ncornr) )
!kkg    allocate( self % ZoneToSrc(Size%nzones) )
!kkg    allocate( self % nfpc(Size%ncornr) )

!   Pointers

    write (6,*) "allocate self % ZData(Size%nzones) "
    allocate( self )
   
    write (6,*) "init     self % ZData(Size%nzones) "
    do zoneID = 1, Size%nzones
        read (10,*)  self%ZData(zoneID)%c0, self%ZData(zoneID)%Connect(1:3,1:Size%maxcf,1:Size%maxCorner) 
    enddo
    write (6,*) "Done init     self % ZData(Size%nzones) "
    return
                                                                                             
  end subroutine Geometry_ctor

!=======================================================================
! get Zone Data interface
!=======================================================================
  function Geometry_getZone(self,zoneID) result(ZData)
 
!    Return a pointer to a zone definition
 
!    variable declarations
     implicit none
 
!    passed variables
     type(Geometry),     intent(in)   :: self
     integer,            intent(in)   :: zoneID
     type(ZoneData),     pointer      :: ZData
 
     ZData => self % ZData(zoneID)
 
     return
 
  end function Geometry_getZone
                                                                                             
!=======================================================================
! destruct interface
!=======================================================================

  subroutine Geometry_dtor(self)

    use Size_mod

    implicit none

!   Passed variables

    type(Geometry),  intent(inout) :: self


!!$    deallocate( self % px )
    deallocate( self % volc )

!   Geometry Specific Arrays

    if (Size%ndim == 1) then
      deallocate( self % rj2 )
      deallocate( self % r2 )
      deallocate( self % hj )
      deallocate( self % areac )
    endif

!   Mesh Connectivity

    deallocate( self % CToFace )
    deallocate( self % CToZone )
    deallocate( self % CToPoint )
    deallocate( self % ZoneToSrc )
    deallocate( self % nfpc )


    return

  end subroutine Geometry_dtor

                                                      
end module Geometry_mod

