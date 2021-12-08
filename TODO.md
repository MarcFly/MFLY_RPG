# Some windows...

## Asset Window
 * Same as Engine, but images be rendered
 * Assets are user assets, not really ALL assets that can be loaded
 * Assets are created by the app and if some external things need to be loaded
   then that will be handled in specific areas (Image depends on character, item, map,...)

# User Library
 * Any Custom File described below, it is not really exportable

## Campaign file format
 * Use JSON as base
 * Hold References to a user library (Units, Encounters,...)
 * Notes and such should not be part of this tool really


## Encounter Format
 * Has a Map - Image
    * Allow multiple floors - 1 Image per floor (force transparencies)
 * Map Scale factor (pixels to feet/meters)
 * Has a DM decided grid attached
 * Hold Unit references

## Unit Format
 * Unit Title
 * HP
 * Allow any text like, description, attacks,...
 * Size (official DnD sizes)
 * Hold a Link to the webpage where it is defined so it is easy to obtain
 * Reference Image
 * Scale + TopLeft to setup the circle to be rendered as a "figure"
 * Notes
    * Specific Name
    * Physical Description
    * Loot
    * Alignment
    * Religion or Beliefs
    * Trait list

## Equipment
 * Similar to a Unit
 * Unit Title
 * Description
 * Image

## Traits - Advantages - Skills - ...
 * Later, not important as it can be found online

# Offline Tools
 * Encounter Creator
 * Unit Creator
 * Unit Manager -> Create specifics of a created Unit
    * Units added to an encounter are duplicated and can be added things

## Encounter Creator
 * Encounter Map
    * Load 1 Image per Floor
 * Unit Selector and finder (Use image setup)
 * Can open Unit Creator
 * Can open Unit Manager

## Unit Creator
 * Follow the Unit format for a window to create such a base character
 * Toggle between Template and Full characte
    * Template Character will not have specific  Notes (loot, physical description,...)
    * Full characters don't actually require specific notes, but the creator will show it
 * 


# NETWORK

# DM Host Game
 * Encounter Play
 * Sound FX playing
 * Music Playing
 * Initiative Tracker
 * Move Units
 * Moves turns and allows players

# Player Join Game
 * Encounter Play -> Fog of War?
 * Character Pages
 * Hear Music and SFX the DM wants to play
 * Can send units to DM (Character, Polymorph options,...)
 * Can only move owned units and when it is their turn
 * Use other things for notes and such