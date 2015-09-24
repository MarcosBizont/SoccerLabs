<?php
/**
 * @file
 * views-aggregator-results-table.tpl.php
 *
 * Template to display views results after aggregation as a table.
 *
 * This template is based on the one in the Views module:
 * views/themes/views-view-table.tpl.php
 *
 * - $title : The title of this group of rows. May be empty.
 * - $header: An array of header labels keyed by field id.
 * - $caption: The caption for this table. May be empty.
 * - $header_classes: An array of header classes keyed by field id.
 * - $fields: An array of CSS IDs to use for each field id.
 * - $classes: A class or classes to apply to the table, based on settings.
 * - $row_classes: An array of classes to apply to each row, indexed by row
 *   number. This matches the index in $rows.
 * - $rows: An array of row items. Each row is an array of content.
 *   $rows are keyed by row number, fields within rows are keyed by field ID.
 * - $field_classes: An array of classes to apply to each field, indexed by
 *   field id, then row number. This matches the index in $rows.
 * - $totals_row_position: whether to show the totals row at top, bottom or both
 * @ingroup views_templates
 */
?>
<div <?php if ($classes): ?>class="<?php print $classes; ?>"<?php endif ?><?php print $attributes; ?>>
  <?php if (!empty($title) || !empty($caption)) : ?>
    <caption><?php print $caption . $title; ?></caption>
  <?php endif; ?>
    <div>
      <?php if (!empty($header)) : ?>
        <div>
          <?php foreach ($header as $field => $label): ?>
            <div <?php if ($header_classes[$field]): ?>class="<?php print $header_classes[$field]; ?>"<?php endif ?>>
              <?php print $label; ?>
            </div>
          <?php endforeach ?>
        </div>
      <?php endif ?>
      <?php if (($totals_row_position & 1) && !empty($totals)) : ?>
        <div>
          <?php
            // Use order of the row fields to output the totals likewise.
            foreach (array_keys(reset($rows)) as $field):
          ?>
            <div <?php if (!empty($field_classes[$field])): ?>class="<?php print reset($field_classes[$field]); ?>"<?php endif ?>>
              <?php print isset($totals[$field]) ? $totals[$field] . ' -  ' .   number_format(($totals[$field]/count($rows))*100,2) . '%' : ''; ?>
            </div>
          <?php endforeach ?>
        </div>
      <?php endif; ?>
    </div>
  <div>
    <?php foreach ($rows as $r => $row): ?>
      <div <?php if (!empty($row_classes[$r])): ?>class="<?php print implode(' ', $row_classes[$r]); ?>"<?php endif ?>>
        <?php foreach ($row as $field => $content): ?>
          <div <?php if (!empty($field_classes[$field][$r])): ?>class="<?php print $field_classes[$field][$r]; ?>"<?php endif ?>
              <?php if (!empty($field_attributes[$field][$r])): ?><?php print drupal_attributes($field_attributes[$field][$r]); ?><?php endif ?>>
            <?php print $content; ?>
          </div>
        <?php endforeach ?>
      </div>
    <?php endforeach ?>
  </div>
  <?php if (($totals_row_position & 2) && !empty($totals)) : ?>
    <div>
      <div>
        <?php
          // Use order of the row fields to output the totals likewise.
          foreach (array_keys(reset($rows)) as $field):
        ?>
          <div <?php if (!empty($field_classes[$field])): ?>class="<?php print reset($field_classes[$field]); ?>"<?php endif ?>>
            <?php print isset($totals[$field]) ? $totals[$field] . ' -  ' .   number_format(($totals[$field]/count($rows))*100,2) . '%' : ''; ?>
          </div>
        <?php endforeach ?>
      </div>
    </div>
  <?php endif ?>
</div>
