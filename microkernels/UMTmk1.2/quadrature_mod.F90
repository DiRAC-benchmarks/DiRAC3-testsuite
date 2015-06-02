! Quadrature Module:  Contains data structures describing angular quadrature 

module Quadrature_mod 

  use kind_mod
  use constant_mod
! use Communicator_mod

  private

! public interfaces

  public construct, destruct
                                                                                 
  type, public :: Quadrature 

     real(adqt), pointer  :: Omega(:,:)        ! direction cosines 

     integer,    pointer :: next(:,:)          ! next(ncornr+1,NumAngles)
     integer,    pointer :: nextZ(:,:)         ! nextZ(nzones,NumAngles)

  end type Quadrature 

  type(Quadrature), pointer, public :: QuadSet

  interface construct
    module procedure Quadrature_ctor
  end interface

  interface destruct
    module procedure Quadrature_dtor
  end interface

contains

!=======================================================================
! construct interface
!=======================================================================
                                                                                   
  subroutine Quadrature_ctor(self, omega,  next, nextZ) 

    use Size_mod

    implicit none

!   Passed variables

    type(Quadrature), intent(inout)    :: self
    integer,    intent(in)    :: next(Size%ncornr+1)
    integer,    intent(in)    :: nextZ(Size%nzones)
    integer,    intent(in)    :: omega(Size%ndim, Size%nangsn)



!   Local
    integer NumberAngles
    integer Ndim

!   Set Properties

    Ndim              = Size% ndim
    NumberAngles = Size%nangsn


    allocate( self % Omega(Ndim,NumberAngles) )
    allocate( self% next(Size%ncornr+1, NumberAngles) )
    allocate( self% nextZ(Size%nzones, NumberAngles) )


    return

  end subroutine Quadrature_ctor

                                                      
!=======================================================================
! destruct interface
!=======================================================================
                                                                                    
  subroutine Quadrature_dtor(self, Ndim)


    implicit none

!   Passed variables
                                                                                     
    type(Quadrature),  intent(inout) :: self
    integer,           intent(in)    :: Ndim

!   Local

    deallocate( self % Omega )
    deallocate( self % next  )
    deallocate( self % nextZ )

    return

  end subroutine Quadrature_dtor




end module Quadrature_mod

