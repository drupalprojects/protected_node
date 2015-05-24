
Protected Node Documentation
----------------------------

 * Original :   http://www.m2osw.com/doc-protected-node
 * Drupal.org : https://drupal.org/node/2176627


Description
-----------

With the protected node module users can restrict access to a node with a
password they provide when creating the node (or a site wide password or a per
node type password.) On node creation authorized authors can protect a node by
supplying a password and verify strength of the password via JavaScript.

People who want to view the node or download one of its private attachments are
first redirected to a password query page (/protected-node). Once the user
entered the right password, he is redirected back to the original node.
Authorizations are stored in sessions, so users don't have to enter the
password over and over again once provided (requires cookies.)

The module includes support for sending emails, views, and rules.


Installation
------------

There is no special requirement for this module.

Once enabled, go to the administer permissions page to set the new permissions.

Below is a matrix with permissions you must set depending on your needs.

Needs:
- 1: No access at all to the resource.
- 2: Need to enter the password to see the resource.
- 3: Can view resource without password, need to enter the password to edit the
resource.
- 4: Bypass completely the protection (can edit and view without password).
- 5: Edit node passwords.

Permissions:
- A: Access protected node password form.
- B: View protected content (bypass password).
- C: Edit protected content (bypass password).
- D: Edit content type password.

Permissions\Cases | 1 | 2 | 3 | 4 | 5 |
------------------|---|---|---|---|---|
         A        |   | 1 | 1 |   |   |
------------------|---|---|---|---|---|
         B        |   |   | 1 |   |   |
------------------|---|---|---|---|---|
         C        |   |   |   | 1 |   |
------------------|---|---|---|---|---|
         D        |   |   |   |   | 1 |

Configuration
-------------

Global settings : /admin/config/content/protected_node

Per content type : admin/structure/types/manage/%content_type
In the fieldset "Protected node settings".

You can set password on the add/edit node form in the fieldset
"Password protect this %type_name".


Email extension
---------------

The module is capable of sending the password via unencrypted emails. This is
useful if you create a page to be viewed only by a few people you know.


Views extension
---------------

The module has a few extensions supporting Views. It includes filters and
output extensions.


Rules extension
---------------

The module includes a Rules extension allowing you to act on a protected node
and test the current state of a node for the current user.


Known conflicts
---------------

 * Login Destination


Similar Modules
---------------

 * Protected Pages
