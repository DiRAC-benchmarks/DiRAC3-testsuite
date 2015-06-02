! Geometry Modules:  geometry and connectivity information used by Sn
                                                                                 
module Size_mod 

  use kind_mod
  use constant_mod

  private

! public interfaces

  public constructSize
                                                                                 
  type, public :: MeshSize
     integer :: my_node   ! node ID
     integer :: nzones    ! number of zones
     integer :: ncornr    ! number of corners
     integer :: nfaces    ! number of zone faces
     integer :: npnts     ! number of points
     integer :: nbelem    ! number of boundary elements
     integer :: ndim      ! number of spatial dimensions
     integer :: maxcf     ! maximum number of zone faces a corner touches
     integer :: maxCorner ! maximum number of corners in a zone
     integer :: ngr       ! number of energy groups
     integer :: nangSN    ! number of angles used for SN sweeps (no acceleration)
     integer :: nangac    ! number of angles used for GTA sweeps
     integer :: npsi      ! number of angles in the flux array PSIR
     integer :: ncomm     ! number of processors to communicate with
     integer :: nbshare   ! number of elements on shared boundaries
     integer :: nbedit    ! number of boundary edits

     real(adqt)       :: tfloor   ! temperature floor
     real(adqt)       :: tmin     ! minimum temperature for time step/convergence control
     real(adqt)       :: wtiso    ! isotropic normalization factor
     real(adqt)       :: radForceMultiplier  ! radiation force multiplier
     real(adqt)       :: tau      ! reciprocal of timestep*SpeedOfLight
     real(adqt)       :: CommTimeCycle
     real(adqt)       :: CommTimeTotal

     character(len=8) :: igeom    ! geometry flag
     character(len=8) :: ittyp    ! time dependence flag
     character(len=8) :: iaccel   ! iterative acceleration flag
     character(len=8) :: iscat    ! scattering flag
     character(len=8) :: itimsrc  ! time-dependent source flag
     character(len=8) :: decomp_s ! spatial decomposition flag

  end type MeshSize 

  type(MeshSize), public :: Size

  interface constructSize
    module procedure Size_ctor
  end interface

contains

!=======================================================================
! construct interface
!=======================================================================

  subroutine Size_ctor(self)

    implicit none
                                                                                         
!   Passed variables

    type(MeshSize),    intent(out) :: self
integer :: my_node

write (6,*) ' Enter size_ctor '
read (11,*) self%my_node
read (11,*) self%nzones  
read (11,*) self%ncornr           
read (11,*) self%nfaces           
read (11,*) self%npnts            
read (11,*) self%nbelem           
read (11,*) self%ndim             
read (11,*) self%maxcf            
read (11,*) self%maxcorner        
read (11,*) self%ngr              
read (11,*) self%nangsn           
read (11,*) self%nangac           
read (11,*) self%npsi             
read (11,*) self%ncomm            
read (11,*) self%nbshare          
read (11,*) self%nbedit           
read (11,*) self%tfloor           
read (11,*) self%tmin             
read (11,*) self%wtiso            
read (11,*) self%radforcemultiplier 
read (11,*) self%tau              
read (11,*) self%commtimecycle    
read (11,*) self%commtimetotal    
read (11,*) self%igeom            
read (11,*) self%ittyp            
read (11,*) self%iaccel           
read (11,*) self%iscat            
read (11,*) self%itimsrc          

write (6,*) "Exit  subroutine size_ctor " 


    return
   
  end subroutine Size_ctor
                                                                                 
                                                      
end module Size_mod

